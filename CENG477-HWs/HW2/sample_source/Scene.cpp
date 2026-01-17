#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>

#include "tinyxml2.h"
#include "Triangle.h"
#include "Helpers.h"
#include "Scene.h"
#include "Vec4WithColor.h"
#include "Vec4.h"

using namespace tinyxml2;
using namespace std;

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *xmlElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *rootNode = xmlDoc.FirstChild();

	// read background color
	xmlElement = rootNode->FirstChildElement("BackgroundColor");
	str = xmlElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	xmlElement = rootNode->FirstChildElement("Culling");
	if (xmlElement != NULL)
	{
		str = xmlElement->GetText();

		if (strcmp(str, "enabled") == 0)
		{
			this->cullingEnabled = true;
		}
		else
		{
			this->cullingEnabled = false;
		}
	}

	// read cameras
	xmlElement = rootNode->FirstChildElement("Cameras");
	XMLElement *camElement = xmlElement->FirstChildElement("Camera");
	XMLElement *camFieldElement;
	while (camElement != NULL)
	{
		Camera *camera = new Camera();

		camElement->QueryIntAttribute("id", &camera->cameraId);

		// read projection type
		str = camElement->Attribute("type");

		if (strcmp(str, "orthographic") == 0)
		{
			camera->projectionType = ORTOGRAPHIC_PROJECTION;
		}
		else
		{
			camera->projectionType = PERSPECTIVE_PROJECTION;
		}

		camFieldElement = camElement->FirstChildElement("Position");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->position.x, &camera->position.y, &camera->position.z);

		camFieldElement = camElement->FirstChildElement("Gaze");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->gaze.x, &camera->gaze.y, &camera->gaze.z);

		camFieldElement = camElement->FirstChildElement("Up");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf", &camera->v.x, &camera->v.y, &camera->v.z);

		camera->gaze = normalizeVec3(camera->gaze);
		camera->u = crossProductVec3(camera->gaze, camera->v);
		camera->u = normalizeVec3(camera->u);

		camera->w = inverseVec3(camera->gaze);
		camera->v = crossProductVec3(camera->u, camera->gaze);
		camera->v = normalizeVec3(camera->v);

		camFieldElement = camElement->FirstChildElement("ImagePlane");
		str = camFieldElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &camera->left, &camera->right, &camera->bottom, &camera->top,
			   &camera->near, &camera->far, &camera->horRes, &camera->verRes);

		camFieldElement = camElement->FirstChildElement("OutputName");
		str = camFieldElement->GetText();
		camera->outputFilename = string(str);

		this->cameras.push_back(camera);

		camElement = camElement->NextSiblingElement("Camera");
	}

	// read vertices
	xmlElement = rootNode->FirstChildElement("Vertices");
	XMLElement *vertexElement = xmlElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (vertexElement != NULL)
	{
		Vec3WithColor *vertex = new Vec3WithColor();
		vertex->vertexId = vertexId;

		str = vertexElement->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = vertexElement->Attribute("color");
		sscanf(str, "%lf %lf %lf", &vertex->color.r, &vertex->color.g, &vertex->color.b);

		this->vertices.push_back(vertex);

		vertexElement = vertexElement->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	xmlElement = rootNode->FirstChildElement("Translations");
	XMLElement *translationElement = xmlElement->FirstChildElement("Translation");
	while (translationElement != NULL)
	{
		Translation *translation = new Translation();

		translationElement->QueryIntAttribute("id", &translation->translationId);

		str = translationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		this->translations.push_back(translation);

		translationElement = translationElement->NextSiblingElement("Translation");
	}

	// read scalings
	xmlElement = rootNode->FirstChildElement("Scalings");
	XMLElement *scalingElement = xmlElement->FirstChildElement("Scaling");
	while (scalingElement != NULL)
	{
		Scaling *scaling = new Scaling();

		scalingElement->QueryIntAttribute("id", &scaling->scalingId);
		str = scalingElement->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		this->scalings.push_back(scaling);

		scalingElement = scalingElement->NextSiblingElement("Scaling");
	}

	// read rotations
	xmlElement = rootNode->FirstChildElement("Rotations");
	XMLElement *rotationElement = xmlElement->FirstChildElement("Rotation");
	while (rotationElement != NULL)
	{
		Rotation *rotation = new Rotation();

		rotationElement->QueryIntAttribute("id", &rotation->rotationId);
		str = rotationElement->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		this->rotations.push_back(rotation);

		rotationElement = rotationElement->NextSiblingElement("Rotation");
	}

	// read meshes
	xmlElement = rootNode->FirstChildElement("Meshes");

	XMLElement *meshElement = xmlElement->FirstChildElement("Mesh");
	while (meshElement != NULL)
	{
		Mesh *mesh = new Mesh();

		meshElement->QueryIntAttribute("id", &mesh->meshId);

		// read mesh faces
		char *row;
		char *cloneStr;
		int vertexId1, vertexId2, vertexId3;
		str = meshElement->GetText();
		cloneStr = strdup(str);

		row = strtok(cloneStr, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &vertexId1, &vertexId2, &vertexId3);

			if (result != EOF)
			{
				Vec3WithColor v1 = *(this->vertices[vertexId1 - 1]);
				Vec3WithColor v2 = *(this->vertices[vertexId2 - 1]);
				Vec3WithColor v3 = *(this->vertices[vertexId3 - 1]);

				mesh->triangles.push_back(Triangle(v1, v2, v3));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		this->meshes.push_back(mesh);

		meshElement = meshElement->NextSiblingElement("Mesh");
	}

	// read instances
	xmlElement = rootNode->FirstChildElement("Instances");

	XMLElement *instanceElement = xmlElement->FirstChildElement("Instance");
	while (instanceElement != NULL)
	{
		Instance *instance = new Instance();
		int meshId;

		instanceElement->QueryIntAttribute("id", &instance->instanceId);
		instanceElement->QueryIntAttribute("meshId", &meshId);

		instance->mesh = *(this->meshes[meshId - 1]);

		// read projection type
		str = instanceElement->Attribute("type");

		if (strcmp(str, "wireframe") == 0)
		{
			instance->instanceType = WIREFRAME_INSTANCE;
		}
		else
		{
			instance->instanceType = SOLID_INSTANCE;
		}

		// read instance transformations
		XMLElement *instanceTransformationsElement = instanceElement->FirstChildElement("Transformations");
		XMLElement *instanceTransformationElement = instanceTransformationsElement->FirstChildElement("Transformation");

		while (instanceTransformationElement != NULL)
		{
			char transformationType;
			int transformationId;

			str = instanceTransformationElement->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			instance->transformationTypes.push_back(transformationType);
			instance->transformationIds.push_back(transformationId);

			instanceTransformationElement = instanceTransformationElement->NextSiblingElement("Transformation");
		}

		instance->numberOfTransformations = instance->transformationIds.size();
		this->instances.push_back(instance);

		instanceElement = instanceElement->NextSiblingElement("Instance");
	}
}

void Scene::assignColorToPixel(int i, int j, Color c)
{
	this->image[i][j].r = c.r;
	this->image[i][j].g = c.g;
	this->image[i][j].b = c.b;
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;
			vector<double> rowOfDepths;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
				rowOfDepths.push_back(1.01);
			}

			this->image.push_back(rowOfColors);
			this->depth.push_back(rowOfDepths);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				assignColorToPixel(i, j, this->backgroundColor);

				this->depth[i][j] = 1.01;
				this->depth[i][j] = 1.01;
				this->depth[i][j] = 1.01;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (vector<vector<Color>>) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFilename.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFilename << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Transformations, clipping, culling, rasterization are done here.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
    Matrix4 M_cam = getCameraTransformationMatrix(camera);
    Matrix4 M_per;
    if (camera->projectionType == ORTOGRAPHIC_PROJECTION) {
        M_per = getOrthographicProjectionMatrix(camera);
    } 
	else {
        M_per = getPerspectiveProjectionMatrix(camera);
    }
    Matrix4 M_vp = getViewportTransformationMatrix(camera);

	for (size_t i = 0; i < instances.size(); i++) {
		Instance* instance = instances[i];
	    Matrix4 M_model = getIdentityMatrix();
		for (size_t j = 0; j < instance->numberOfTransformations; j++) {
            char type = instance->transformationTypes[j];
            int id = instance->transformationIds[j];
			Matrix4 M_current = getIdentityMatrix();
            if (type == 't') {
                Translation *t = translations[id - 1]; // since IDs start from 1
                M_current = getTranslationMatrix(t->tx, t->ty, t->tz);
            } 
			else if (type == 's') {
                Scaling *s = scalings[id - 1];
                M_current = getScalingMatrix(s->sx, s->sy, s->sz);
            } 
			else if (type == 'r') {
                Rotation *r = rotations[id - 1];
                M_current = getRotationMatrix(r->angle, r->ux, r->uy, r->uz);
            }
            M_model = multiplyMatrixWithMatrix(M_current, M_model);
        }

        Matrix4 M_world_to_cam = multiplyMatrixWithMatrix(M_cam, M_model);

		for (size_t k = 0; k < instance->mesh.triangles.size(); k++) {
			Triangle triangle = instance->mesh.triangles[k];

            Vec3WithColor v1_orig = triangle.v1; // get original 3D vertex data
            Vec3WithColor v2_orig = triangle.v2;
            Vec3WithColor v3_orig = triangle.v3;

            Vec4WithColor v1_h(v1_orig.x, v1_orig.y, v1_orig.z, 1.0, v1_orig.color); // convert to homogeneous coordinates
            Vec4WithColor v2_h(v2_orig.x, v2_orig.y, v2_orig.z, 1.0, v2_orig.color);
            Vec4WithColor v3_h(v3_orig.x, v3_orig.y, v3_orig.z, 1.0, v3_orig.color);

            Vec4WithColor v1_cam_h = multiplyMatrixWithVec4WithColor(M_world_to_cam, v1_h); // transform to camera space
            Vec4WithColor v2_cam_h = multiplyMatrixWithVec4WithColor(M_world_to_cam, v2_h);
            Vec4WithColor v3_cam_h = multiplyMatrixWithVec4WithColor(M_world_to_cam, v3_h);

			Vec3 v1_cam(v1_cam_h.x, v1_cam_h.y, v1_cam_h.z);
			Vec3 v2_cam(v2_cam_h.x, v2_cam_h.y, v2_cam_h.z);
			Vec3 v3_cam(v3_cam_h.x, v3_cam_h.y, v3_cam_h.z);

            if (cullingEnabled) {
                if (isBackface(v1_cam, v2_cam, v3_cam)) {
                    continue;
                }
            }

            Vec4WithColor v1_clip = multiplyMatrixWithVec4WithColor(M_per, v1_cam_h); // transform to clip space
            Vec4WithColor v2_clip = multiplyMatrixWithVec4WithColor(M_per, v2_cam_h);
            Vec4WithColor v3_clip = multiplyMatrixWithVec4WithColor(M_per, v3_cam_h);

            if (instance->instanceType == WIREFRAME_INSTANCE) { // wireframe mode: clip lines
                std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}};
                std::vector<Vec4WithColor> clip_verts = {v1_clip, v2_clip, v3_clip};

                for (int e = 0; e < edges.size(); e++) {
                    Vec4WithColor v_a_clip = clip_verts[edges[e].first];
                    Vec4WithColor v_b_clip = clip_verts[edges[e].second];
                    
                    Vec4WithColor v_a_out, v_b_out;

                    // Liang-Barsky implementation
                    if (clipLine(v_a_clip, v_b_clip, v_a_out, v_b_out)) {
                        // perspective division (clip to normalized_coords)
                        Vec4WithColor v_a_normalized_coords(v_a_out.x / v_a_out.t, v_a_out.y / v_a_out.t, v_a_out.z / v_a_out.t, 1.0, v_a_out.color);
                        Vec4WithColor v_b_normalized_coords(v_b_out.x / v_b_out.t, v_b_out.y / v_b_out.t, v_b_out.z / v_b_out.t, 1.0, v_b_out.color);

                        // viewport transformation (normalized_coords to viewport)
                        Vec4WithColor v_a_vp = multiplyMatrixWithVec4WithColor(M_vp, v_a_normalized_coords);
                        Vec4WithColor v_b_vp = multiplyMatrixWithVec4WithColor(M_vp, v_b_normalized_coords);

                        // rasterization (midpoint line)
                        drawLine((int)(v_a_vp.x), (int)(v_a_vp.y), v_a_vp.z, v_a_vp.color, (int)(v_b_vp.x), (int)(v_b_vp.y), v_b_vp.z, v_b_vp.color, this, camera->horRes, camera->verRes);
                    }
                }

            } 

			else if (instance->instanceType == SOLID_INSTANCE) { // solid mode: no clipping
				// solid mode trivial reject            
                std::vector<Vec4WithColor> clip_verts = {v1_clip, v2_clip, v3_clip};
                if (clip_verts[0].x < -clip_verts[0].t && clip_verts[1].x < -clip_verts[1].t && clip_verts[2].x < -clip_verts[2].t) continue; // all outside left
                if (clip_verts[0].x > clip_verts[0].t && clip_verts[1].x > clip_verts[1].t && clip_verts[2].x > clip_verts[2].t) continue; // all outside right
                if (clip_verts[0].y < -clip_verts[0].t && clip_verts[1].y < -clip_verts[1].t && clip_verts[2].y < -clip_verts[2].t) continue; // all outside bottom
                if (clip_verts[0].y > clip_verts[0].t && clip_verts[1].y > clip_verts[1].t && clip_verts[2].y > clip_verts[2].t) continue; // all outside top
                if (clip_verts[0].z < -clip_verts[0].t && clip_verts[1].z < -clip_verts[1].t && clip_verts[2].z < -clip_verts[2].t) continue; // all outside near
                if (clip_verts[0].z > clip_verts[0].t && clip_verts[1].z > clip_verts[1].t && clip_verts[2].z > clip_verts[2].t) continue; // all outside far

				// perspective division (clip to normalized_coords)
                Vec4WithColor v1_normalized_coords(v1_clip.x / v1_clip.t, v1_clip.y / v1_clip.t, v1_clip.z / v1_clip.t, 1.0, v1_clip.color);
                Vec4WithColor v2_normalized_coords(v2_clip.x / v2_clip.t, v2_clip.y / v2_clip.t, v2_clip.z / v2_clip.t, 1.0, v2_clip.color);
                Vec4WithColor v3_normalized_coords(v3_clip.x / v3_clip.t, v3_clip.y / v3_clip.t, v3_clip.z / v3_clip.t, 1.0, v3_clip.color);

                // viewport transformation (normalized_coords to viewport)
                Vec4WithColor v1_vp = multiplyMatrixWithVec4WithColor(M_vp, v1_normalized_coords);
                Vec4WithColor v2_vp = multiplyMatrixWithVec4WithColor(M_vp, v2_normalized_coords);
                Vec4WithColor v3_vp = multiplyMatrixWithVec4WithColor(M_vp, v3_normalized_coords);

                // rasterization (barycentric)
                rasterizeTriangle(v1_vp, v2_vp, v3_vp, this, camera->horRes, camera->verRes);
            }
        }
    }
}
