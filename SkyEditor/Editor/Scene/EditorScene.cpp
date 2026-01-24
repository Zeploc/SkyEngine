// Copyright Skyward Studios, Inc. All Rights Reserved.

// Library Includes //
#include "SEPCH.h"
#include "EditorScene.h"

#include "Render/Lighting.h"

// Engine Includes //
#include <fstream>

#include "Editor/Windows/EditorWindow.h"
#include "Editor/Windows/EditorWindowManager.h"
#include <Render/Meshes/Basic/Plane.h>

#include <Windows.h>

#include "Core/Application.h"
#include "Editor/EditorViewportCanvas.h"
#include "Entity/Entity.h"
#include "Platform/Window/EngineWindow.h"
#include "Render/Materials/Material.h"
#include "Render/Meshes/BoxComponent.h"
#include "Render/Meshes/Basic/DefaultMeshes.h"

EditorScene::EditorScene(const std::string& InSceneName) : Scene(InSceneName)
{
	const TSharedPointer<CEngineWindow> ApplicationWindow = GetApplication()->GetApplicationWindow();
	const SVector2i WindowSize = ApplicationWindow->GetSize();	

	// TSharedPointer<UIText> TipText(new UIText({WindowSize.X - 20, 15.0f}, 0, {0.3, 0.3, 0.3, 1.0f}, "G - Wireframe  |  WASD - Move  |  Mouse - Look  |  Space - Jump  |  ESC - Mouse Toggle", "Resources/Fonts/Roboto-Regular.ttf", 22, EANCHOR::TOP_RIGHT));
	// AddUIElement(TipText);
	
	EditorWindow* NewWindow = new EditorWindow("Outliner", ApplicationWindow, glm::vec2(300, 400), glm::vec2(0, 0));
	NewWindow->SetBackColour(glm::vec3(0.2, 0.6, 0.8));
		
	EditorWindow* ContentWindow = new EditorWindow("Content", ApplicationWindow, glm::vec2(600, 150), glm::vec2(0, WindowSize.Y - 150));
	ContentWindow->SetBackColour(glm::vec3(0.4, 0.4, 0.4));

	//EditorWindow* ExternalWindow = new EditorWindow("External Test", nullptr, glm::vec2(500, 300), glm::vec2(100, 100));
	//ExternalWindow->SetBackColour(glm::vec3(0.6, 0.3, 0.4));
}

void EditorScene::OnLoaded()
{
	Scene::OnLoaded();
	AddSampleEntities();
}

void EditorScene::AddSampleEntities()
{
	TAssetObjectPointer<CMaterialInterface> ColouredBrickMaterial = CMaterialUtils::FindMaterial("ColouredBrickMaterial");
	TAssetObjectPointer<CMaterialInterface> ColouredBrickPlaneMaterial = CMaterialUtils::FindMaterial("ColouredBrickPlaneMaterial");
	TAssetObjectPointer<CMaterialInterface> CliffMaterial = CMaterialUtils::FindMaterial("CliffMaterial");
	TAssetObjectPointer<CMaterialInterface> PlaneMaterial = CMaterialUtils::FindMaterial("PlaneMaterial");
	TAssetObjectPointer<CMaterialInterface> BoxMaterial = CMaterialUtils::FindMaterial("BoxMaterial");
	
	TSharedPointer<Entity> SphereRaycastTest(new Entity(STransform{{18.0f, 2.0f, 0.0f}, {0, 0, 0}, {2, 2, 2}}, "Sphere"));
	TSharedPointer<CMeshComponent> SphereRaycastMesh = std::make_shared<CMeshComponent>(SphereRaycastTest, DefaultMesh::GetSphere(), CliffMaterial);
	SphereRaycastTest->AddComponent(SphereRaycastMesh);
	AddEntity(SphereRaycastTest);
			
	TSharedPointer<Entity> FloorEntity(new Entity({{0, 0, 0}, {0, -90, 0}, {50}}, "Floor Plane"));
	//glm::vec3 Points[4] = { {-10, 10, 1}, {10, 10, -1 }, { 10, -10, 0 }, { -10, -10, -3 } };
	const TSharedPointer<CMeshComponent> FloorPlanMesh = std::make_shared<CMeshComponent>(FloorEntity, DefaultMesh::GetPlane(), PlaneMaterial);
	FloorEntity->AddComponent(FloorPlanMesh);
	AddEntity(FloorEntity);
	
	// TSharedPointer<ParticleSystem> ParticleBoy(new ParticleSystem({{20, 8, 10}, {0, 0, 0}, {1, 1, 1}}));
	// ParticleBoy->SetPositionRange({-5, 5}, {0, 0}, {-5, 5});
	// ParticleBoy->SetDirectionRange({0, 0}, {-1, -1}, {0, 0});
	// ParticleBoy->SetFalloffTime({100.0f, 100.0f});
	// ParticleBoy->SetFalloffRange({30, 40});
	// ParticleBoy->SetSpeedRange({1, 10});
	// ParticleBoy->ParticleSize = 0.8f;
	// ParticleBoy->Init(1000, "Resources/Images/raindrop.png");
	// AddEntity(ParticleBoy, true);
	//
	// TSharedPointer<ParticleSystem> ParticleBoy2(new ParticleSystem({{20, 1, 20}, {0, 0, 0}, {1, 1, 1}}));
	// ParticleBoy2->Init(1000, "Resources/Images/Box.png");
	// AddEntity(ParticleBoy2, true);
	//
	// TSharedPointer<Entity> GeomEnt(new Entity({{10, 6, 10}, {0, 0, 0}, {1, 1, 1}}, EANCHOR::CENTER));
	// TSharedPointer<GeometryObject> GeomShape = std::make_shared<GeometryObject>(GeomEnt, SVector4(0.0, 0.9f, 0.3f, 1.0f));
	// GeomEnt->AddMesh(GeomShape);
	// AddEntity(GeomEnt, true);
	
	// TODO: Fix as broken with updated assimp
	// Pointer<Entity> ModelEntity(new Entity(FTransform{{10.0f, 2.0f, 0.0f}, {-90, -90, 0}, {0.2f, 0.2f, 0.2f}}, EANCHOR::CENTER));
	// Pointer<Model> ModelEntityMesh(new Model({1.0f, 1.0f, 1.0f, 1.0f}, "Resources/Models/theDude.DAE"));
	// ModelEntity->AddMesh(ModelEntityMesh);
	// ModelEntityMesh->SetLit(true);
	// ModelEntityMesh->SetReflection(true);
	// AddEntity(ModelEntity, true);
	
	TSharedPointer<Entity> CubeEnty(new Entity(STransform{{10.0f, 4.0f, 4.0f}, {0, 0, 0}, {3, 3, 3}}, "Cube"));
	TSharedPointer<CMeshComponent> CubyMesh(new CMeshComponent(CubeEnty, DefaultMesh::GetCube(), ColouredBrickMaterial));
	CubeEnty->AddComponent(CubyMesh);
	AddEntity(CubeEnty);
	
	TSharedPointer<Entity> PyramidEntity(new Entity(STransform{{10.0f, 4.0f, 8.0f}, {0, 0, 0}, {3, 3, 3}}, "Pyramid"));
	TSharedPointer<CMeshComponent> PyramidMesh(new CMeshComponent(PyramidEntity, DefaultMesh::GetPyramid(), ColouredBrickMaterial));
	PyramidEntity->AddComponent(PyramidMesh);
	// PyramidMesh->SetLit(false, true);
	// TODO: Identify and fix pyramid lighting
	AddEntity(PyramidEntity);
	
	TSharedPointer<Entity> SphereEntity(new Entity(STransform{{10.0f, 4.0f, 12.0f}, {0, 0, 0}, {2, 2, 2}}, "Sphere 2"));
	TSharedPointer<CMeshComponent> SphereMesh(new CMeshComponent(SphereEntity, DefaultMesh::GetSphere(), ColouredBrickMaterial));
	SphereEntity->AddComponent(SphereMesh);
	AddEntity(SphereEntity);
	
	TSharedPointer<Entity> PlaneEntity = std::make_shared<Entity>(STransform{{10.0f, 4.0f, 16.0f}, {0, -90, 0}, {2, 2, 2}}, "Plane");
	TSharedPointer<CPlane> PlaneMesh = CreatePointer<CPlane>(PlaneEntity, ColouredBrickPlaneMaterial);
	PlaneEntity->AddComponent(PlaneMesh);
	AddEntity(PlaneEntity);
	
	TSharedPointer<Entity> BoxEntity = std::make_shared<Entity>(STransform{{5.0f, 5.0f, 10.0f}, {0, 0, 0}, {2, 2, 2}}, "Box");
	TSharedPointer<CBoxComponent> BoxComponent = CreatePointer<CBoxComponent>(BoxEntity, BoxMaterial);
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
