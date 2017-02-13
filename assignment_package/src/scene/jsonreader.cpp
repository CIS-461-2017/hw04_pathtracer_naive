#include <scene/jsonreader.h>
#include <scene/geometry/mesh.h>
#include <scene/geometry/cube.h>
#include <scene/geometry/sphere.h>
#include <scene/geometry/squareplane.h>
#include <iostream>

/*
void JSONReader::LoadSceneFromFile(QFile &file, const QStringRef &local_path, Scene &scene)
{
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray rawData = file.readAll();
        // Parse document
        QJsonDocument doc(QJsonDocument::fromJson(rawData));

        // Get JSON object
        QJsonObject json = doc.object();

        QJsonObject sceneObj, camera;
        QJsonArray geometryList, materialList;

        QMap<QString, QList<Primitive*>> material_to_geometry_map;
        sceneObj = json["scene"].toObject();
        if(sceneObj.contains(QString("camera"))) {
            camera = sceneObj["camera"].toObject();
            scene.SetCamera(LoadCamera(camera));
        }
        if(sceneObj.contains("geometry")) {
            geometryList = sceneObj["geometry"].toArray();
            foreach (const QJsonValue & geometryVal, geometryList) {
                QJsonObject geometryObj = geometryVal.toObject();
                Primitive* geometry = LoadGeometry(geometryObj, material_to_geometry_map, local_path);
                if(geometry == NULL) return;
                scene.objects.append(geometry);
            }
        }
        if(sceneObj.contains(QString("material"))){
            materialList = sceneObj["material"].toArray();
            foreach (const QJsonValue & materialVal, materialList) {
                QJsonObject materialObj = materialVal.toObject();
                Material* material = LoadMaterial(materialObj, local_path);
                if(material == NULL) return;
                scene.materials.append(material);
            }
        }
        if(sceneObj.contains(QString("integrator"))) {
            QJsonObject integratorObj = sceneObj["integrator"].toObject();
            integrator = LoadIntegrator(integratorObj);
        }
        //Associate the materials in the json file with the geometries that use those materials.
        for(int i = 0; i < scene.materials.size(); i++)
        {
            QList<Primitive*> l = material_to_geometry_map.value(scene.materials[i]->name);
            for(int j = 0; j < l.size(); j++)
            {
                l[j]->SetMaterial(scene.materials[i]);
            }
        }

        //Copy emissive geometry from the list of objects to the list of lights
        QList<Primitive*> to_lights;
        for(Geometry *g : scene.objects)
        {
            g->create();
            if(g->material->emissive)
            {
                to_lights.append(g);
            }
        }
        for(Geometry *g : to_lights)
        {
            scene.lights.append(g);
        }
        file.close();
    }
}

Primitive* JSONReader::LoadGeometry(QJsonObject &geometry, QMap<QString, QList<Primitive*>> &map, const QStringRef &local_path)
{
    Primitive* result = NULL;

    //First check what type of geometry we're supposed to load
    QString type;
    if(geometry.contains(QString("type"))){
        type = geometry["type"].toString();
    }

    bool is_mesh = false;
    if(QString::compare(type, QString("obj")) == 0)
    {
        result = new Mesh();
        is_mesh = true;
    }
    else if(QString::compare(type, QString("sphere")) == 0)
    {
        result = new Sphere();
    }
    else if(QString::compare(type, QString("square")) == 0)
    {
        result = new SquarePlane();
    }
    else if(QString::compare(type, QString("cube")) == 0)
    {
        result = new Cube();
    }
    else
    {
        std::cout << "Could not parse the geometry!" << std::endl;
        return NULL;
    }
    if(geometry.contains(QString("name"))) result->name = geometry["name"].toString();

    if(is_mesh && geometry.contains(QString("filename")))
    {
        QString objFilePath = geometry["filename"].toString();
        ((Mesh*)result)->LoadOBJ(QStringRef(&objFilePath), local_path);
    }

    if(geometry.contains(QString("transform"))) {
        QJsonObject transform = geometry["transform"].toObject();
        result->transform = LoadTransform(transform);
    }

    if(geometry.contains(QString("material"))) {
        //Add the geometry to the map of material names to geometries so that we can assign it a material later
        QString material_name = geometry["material"].toString();
        QList<Primitive*> list = map.value(material_name);
        list.append(result);
        map.insert(material_name, list);
    }
    return result;
}

Material* JSONReader::LoadMaterial(QJsonObject &material, const QStringRef &local_path)
{
    Material* result;
    QString type;

    //First check what type of material we're supposed to load
    if(material.contains(QString("type"))) type = material["type"].toString();

    if(QString::compare(type, QString("lambert")) == 0)
    {
        result = new LambertMaterial();
    }
    else if(QString::compare(type, QString("phong")) == 0)
    {
        result = new PhongMaterial();
        float spec_pow = material["specularPower"].toDouble();
        ((PhongMaterial*)result)->specular_power = spec_pow;
    }
    else
    {
        std::cout << "Could not parse the material!" << std::endl;
        return NULL;
    }

    result->name = material["name"].toString();

    if(material.contains(QString("baseColor"))) result->base_color = ToVec3(material["baseColor"].toArray());
    if(material.contains(QString("reflectivity"))) result->reflectivity = (material["reflectivity"].toDouble());
    if(material.contains(QString("iorIn"))) result->refract_idx_in = material["iorIn"].toDouble();
    if(material.contains(QString("iorOut"))) result->refract_idx_out = material["iorOut"].toDouble();
    if(material.contains(QString("texture")))
    {
        QString img_filepath = local_path.toString().append(material["texture"].toString());
        QImage* texture = new QImage(img_filepath);
        result->texture = texture;
    }
    if(material.contains(QString("normalMap")))
    {
        QString img_filepath = local_path.toString().append(material["normalMap"].toString());
        QImage* texture = new QImage(img_filepath);
        result->normal_map = texture;
    }
    result->emissive = material.contains("emissive") ? true : false;
    return result;
}

Camera JSONReader::LoadCamera(QJsonObject& camera)
{
    Camera result;
    if(camera.contains(QString("lookAt"))) result.ref = ToVec3(camera["lookAt"].toArray());
    if(camera.contains(QString("eye"))) result.eye = ToVec3(camera["eye"].toArray());
    if(camera.contains(QString("worldUp"))) result.world_up = ToVec3(camera["worldUp"].toArray());
    if(camera.contains(QString("width"))) result.width = camera["width"].toDouble();
    if(camera.contains(QString("height"))) result.height = camera["height"].toDouble();
    if(camera.contains(QString("fov"))) result.fovy = camera["fov"].toDouble();
    if(camera.contains(QString("nearClip"))) result.near_clip = camera["nearClip"].toDouble();
    if(camera.contains(QString("farClip"))) result.far_clip = camera["farClip"].toDouble();

    result.RecomputeAttributes();
    return result;
}

Transform JSONReader::LoadTransform(QJsonObject &transform)
{
    glm::vec3 t, r, s;
    s = glm::vec3(1,1,1);
    if(transform.contains(QString("translate"))) t = ToVec3(transform["translate"].toArray());
    if(transform.contains(QString("rotate"))) r = ToVec3(transform["rotate"].toArray());
    if(transform.contains(QString("scale"))) s = ToVec3(transform["scale"].toArray());
    return Transform(t, r, s);
}

Integrator JSONReader::LoadIntegrator(QJsonObject &integratorObj)
{
    Integrator result;
    QString type;
    //First check what type of integrator we're supposed to load
    if(integratorObj.contains(QString("type"))) type = integratorObj["type"].toString();
    //do something with the integrator type
    result.SetDepth(integratorObj["maxDepth"].toInt());

    return result;
}

glm::vec3 JSONReader::ToVec3(const QJsonArray &s)
{
    glm::vec3 result(s.at(0).toDouble(), s.at(1).toDouble(), s.at(2).toDouble());
    return result;
}

glm::vec3 JSONReader::ToVec3(const QStringRef &s)
{
    glm::vec3 result;
    int start_idx;
    int end_idx = -1;
    for(int i = 0; i < 3; i++){
        start_idx = ++end_idx;
        while(end_idx < s.length() && s.at(end_idx) != QChar(' '))
        {
            end_idx++;
        }
        result[i] = s.mid(start_idx, end_idx - start_idx).toFloat();
    }
    return result;
}
*/
