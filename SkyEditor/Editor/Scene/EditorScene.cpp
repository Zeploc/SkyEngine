// Copyright Skyward Studios, Inc. All Rights Reserved.

// Library Includes //
#include "SEPCH.h"
#include "EditorScene.h"

#include "Render/Lighting.h"

// Engine Includes //
#include <fstream>

#include "Editor/Windows/EditorWindow.h"
#include "Editor/Windows/EditorWindowManager.h"
#include <Render/Particle/ParticleSystem.h>
#include <Render/Meshes/Basic/Plane.h>

#include <Windows.h>

#include "Core/Application.h"
#include "Editor/EditorViewportCanvas.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Materials/Material.h"
#include "Render/Meshes/BoxComponent.h"
#include "Render/Meshes/MeshManager.h"

EditorScene::EditorScene(const std::string& InSceneName) : Scene(InSceneName)
{
	const TPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i WindowSize = ApplicationWindow->GetSize();	

	// TPointer<UIText> TipText(new UIText({WindowSize.X - 20, 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "G - Wireframe  |  WASD - Move  |  Mouse - Look  |  Space - Jump  |  ESC - Mouse Toggle", "Resources/Fonts/Roboto-Regular.ttf", 22, EANCHOR::TOP_RIGHT));
	// AddUIElement(TipText);
	
	EditorWindow* NewWindow = new EditorWindow("Outliner", ApplicationWindow, glm::vec2(300, 400), glm::vec2(0, 0));
	NewWindow->SetBackColour(glm::vec3(0.2, 0.6, 0.8));
		
	EditorWindow* ContentWindow = new EditorWindow("Content", ApplicationWindow, glm::vec2(600, 150), glm::vec2(0, WindowSize.Y - 150));
	ContentWindow->SetBackColour(glm::vec3(0.4, 0.4, 0.4));

	//EditorWindow* ExternalWindow = new EditorWindow("External Test", nullptr, glm::vec2(500, 300), glm::vec2(100, 100));
	//ExternalWindow->SetBackColour(glm::vec3(0.6, 0.3, 0.4));
}

void EditorScene::OnLoadScene()
{
	Scene::OnLoadScene();
	AddSampleEntities();
}

void EditorScene::AddSampleEntities()
{
	TPointer<CMaterialInterface> ColouredBrickMaterial = GetMaterialManager()->FindMaterial("ColouredBrickMaterial");
	TPointer<CMaterialInterface> ColouredBrickPlaneMaterial = GetMaterialManager()->FindMaterial("ColouredBrickPlaneMaterial");
	TPointer<CMaterialInterface> CliffMaterial = GetMaterialManager()->FindMaterial("CliffMaterial");
	TPointer<CMaterialInterface> PlaneMaterial = GetMaterialManager()->FindMaterial("PlaneMaterial");
	TPointer<CMaterialInterface> BoxMaterial = GetMaterialManager()->FindMaterial("BoxMaterial");
	
	TPointer<Entity> SphereRaycastTest(new Entity(STransform{{18.0f, 2.0f, 0.0f}, {0, 0, 0}, {2, 2, 2}}, "Sphere"));
	TPointer<CMeshComponent> SphereRaycastMesh = std::make_shared<CMeshComponent>(SphereRaycastTest, MESH_SPHERE, CliffMaterial);
	SphereRaycastTest->AddComponent(SphereRaycastMesh);
	AddEntity(SphereRaycastTest);
			
	TPointer<Entity> FloorEntity(new Entity({{0, 0, 0}, {0, -90, 0}, {50}}, "Floor Plane"));
	//glm::vec3 Points[4] = { {-10, 10, 1}, {10, 10, -1 }, { 10, -10, 0 }, { -10, -10, -3 } };
	const TPointer<CMeshComponent> FloorPlanMesh = std::make_shared<CMeshComponent>(FloorEntity, MESH_PLANE, PlaneMaterial);
	FloorEntity->AddComponent(FloorPlanMesh);
	AddEntity(FloorEntity);
	
	// TPointer<ParticleSystem> ParticleBoy(new ParticleSystem({{20, 8, 10}, {0, 0, 0}, {1, 1, 1}}));
	// ParticleBoy->SetPositionRange({-5, 5}, {0, 0}, {-5, 5});
	// ParticleBoy->SetDirectionRange({0, 0}, {-1, -1}, {0, 0});
	// ParticleBoy->SetFalloffTime({100.0f, 100.0f});
	// ParticleBoy->SetFalloffRange({30, 40});
	// ParticleBoy->SetSpeedRange({1, 10});
	// ParticleBoy->ParticleSize = 0.8f;
	// ParticleBoy->Init(1000, "Resources/Images/raindrop.png");
	// AddEntity(ParticleBoy, true);
	//
	// TPointer<ParticleSystem> ParticleBoy2(new ParticleSystem({{20, 1, 20}, {0, 0, 0}, {1, 1, 1}}));
	// ParticleBoy2->Init(1000, "Resources/Images/Box.png");
	// AddEntity(ParticleBoy2, true);
	//
	// TPointer<Entity> GeomEnt(new Entity({{10, 6, 10}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	// TPointer<GeometryObject> GeomShape = std::make_shared<GeometryObject>(GeomEnt, SVector4(0.0, 0.9f, 0.3f, 1.0f));
	// GeomEnt->AddMesh(GeomShape);
	// AddEntity(GeomEnt, true);
	
	// TODO: Fix as broken with updated assimp
	// Pointer<Entity> ModelEntity(new Entity(FTransform{{10.0f, 2.0f, 0.0f}, {-90, -90, 0}, {0.2f, 0.2f, 0.2f}}, EANCHOR::CENTER));
	// Pointer<Model> ModelEntityMesh(new Model({1.0f, 1.0f, 1.0f, 1.0f}, "Resources/Models/theDude.DAE"));
	// ModelEntity->AddMesh(ModelEntityMesh);
	// ModelEntityMesh->SetLit(true);
	// ModelEntityMesh->SetReflection(true);
	// AddEntity(ModelEntity, true);
	
	TPointer<Entity> CubeEnty(new Entity(STransform{{10.0f, 4.0f, 4.0f}, {0, 0, 0}, {3, 3, 3}}, "Cube"));
	TPointer<CMeshComponent> CubyMesh(new CMeshComponent(CubeEnty, MESH_CUBE, ColouredBrickMaterial));
	CubeEnty->AddComponent(CubyMesh);
	AddEntity(CubeEnty);
	
	TPointer<Entity> PyramidEntity(new Entity(STransform{{10.0f, 4.0f, 8.0f}, {0, 0, 0}, {3, 3, 3}}, "Pyramid"));
	TPointer<CMeshComponent> PyramidMesh(new CMeshComponent(PyramidEntity, MESH_PYRAMID, ColouredBrickMaterial));
	PyramidEntity->AddComponent(PyramidMesh);
	// PyramidMesh->SetLit(false, true);
	// TODO: Identify and fix pyramid lighting
	AddEntity(PyramidEntity);
	
	TPointer<Entity> SphereEntity(new Entity(STransform{{10.0f, 4.0f, 12.0f}, {0, 0, 0}, {2, 2, 2}}, "Sphere 2"));
	TPointer<CMeshComponent> SphereMesh(new CMeshComponent(SphereEntity, MESH_SPHERE, ColouredBrickMaterial));
	SphereEntity->AddComponent(SphereMesh);
	AddEntity(SphereEntity);
	
	TPointer<Entity> PlaneEntity = std::make_shared<Entity>(STransform{{10.0f, 4.0f, 16.0f}, {0, -90, 0}, {2, 2, 2}}, "Plane");
	TPointer<CPlane> PlaneMesh = CreatePointer<CPlane>(PlaneEntity, ColouredBrickPlaneMaterial);
	PlaneEntity->AddComponent(PlaneMesh);
	AddEntity(PlaneEntity);
	
	TPointer<Entity> BoxEntity = std::make_shared<Entity>(STransform{{5.0f, 5.0f, 10.0f}, {0, 0, 0}, {2, 2, 2}}, "Box");
	TPointer<CBoxComponent> BoxComponent = CreatePointer<CBoxComponent>(BoxEntity, BoxMaterial);
	BoxEntity->AddComponent(BoxComponent);
	// GetRenderer()->AddBox(GetInterface<ISceneVisual>(BoxComponent));
	AddEntity(BoxEntity);
}

void EditorScene::Update()
{	
	Scene::Update();	
	// TODO: Change later?
	EditorWindowManager::UpdateWindows();
}

void EditorScene::RenderScene()
{
	Scene::RenderScene();
}
