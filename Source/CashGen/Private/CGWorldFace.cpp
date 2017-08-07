#include "cashgen.h"
#include "RuntimeMeshComponent.h"
#include "CGWorldFaceJob.h"
#include "CGWorldFace.h"

ACGWorldFace::ACGWorldFace(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
}

ACGWorldFace::~ACGWorldFace()
{

}

void ACGWorldFace::BeginPlay()
{
	//String compName = "RMC";
	MeshComponent = NewObject<URuntimeMeshComponent>(this, URuntimeMeshComponent::StaticClass());//, *compName);
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeshComponent->BodyInstance.SetResponseToAllChannels(ECR_Block);
	MeshComponent->bShouldSerializeMeshData = false;
	MeshComponent->RegisterComponent();

}

void ACGWorldFace::SubDivideGeometry(const FRuntimeMeshVertexSimple &v1, const FRuntimeMeshVertexSimple &v2, const FRuntimeMeshVertexSimple &v3, const int32 aDepth, const float aScale)
{
	if (aDepth == 0) {
		MyVertices.Emplace(v1);
		MyVertices.Emplace(v2);
		MyVertices.Emplace(v3);

		MyIndices.Add(MyVertices.Num() - 3);
		MyIndices.Add(MyVertices.Num() - 2);
		MyIndices.Add(MyVertices.Num() - 1);
		return;
	}
	const FRuntimeMeshVertexSimple v12 = FRuntimeMeshVertexSimple(((v1.Position + v2.Position).GetSafeNormal()) * aScale);
	const FRuntimeMeshVertexSimple v23 = FRuntimeMeshVertexSimple(((v2.Position + v3.Position).GetSafeNormal()) * aScale);
	const FRuntimeMeshVertexSimple v31 = FRuntimeMeshVertexSimple(((v3.Position + v1.Position).GetSafeNormal()) * aScale);
	SubDivideGeometry(v1, v12, v31, aDepth - 1, aScale);
	SubDivideGeometry(v2, v23, v12, aDepth - 1, aScale);
	SubDivideGeometry(v3, v31, v23, aDepth - 1, aScale);
	SubDivideGeometry(v12, v23, v31, aDepth - 1, aScale);
}

void ACGWorldFace::Tick(float DeltaSeconds)
{


}

void ACGWorldFace::SetupFace(FRuntimeMeshVertexSimple v1, FRuntimeMeshVertexSimple v2, FRuntimeMeshVertexSimple v3, const FVector aOrigin, int32 aSubDivisions , const int32 aSubDivLimit, const float aRadius, ACGWorld* aWorld, ACGWorldFace* aParentFace)
{
	MyWorld = aWorld;
	MyParentFace = aParentFace;
	// We need to spawn more faces rather than try to do this all at once
	if (aSubDivisions > aSubDivLimit)
	{
		// Subdivide once to get all our points
		SubDivideGeometry(v1, v2, v3, 1, aRadius);
		aSubDivisions = aSubDivisions - aSubDivLimit;

		UWorld* _world = GetWorld();

		for (int i = 0; i < 4; i++)
		{
			ACGWorldFace* _newFace = _world->SpawnActor<ACGWorldFace>(ACGWorldFace::StaticClass(), FVector(0.0f), FRotator(0.0f));
			MyWorld->AddFace(_newFace);

			_newFace->SetupFace(MyVertices[MyIndices[i * 3]],
				MyVertices[MyIndices[(i * 3) + 1]],
				MyVertices[MyIndices[(i * 3) + 2]],
				FVector(0.0f),
				aSubDivisions,
				aSubDivLimit,
				aRadius, 
				aWorld,
				this);
		}
	}
	else
	{
		// Otherwise, we will be rendering this face, so enqueue a generation job
		FCGWorldFaceJob newJob;
		newJob.pFace = this;
		newJob.Radius = aRadius;
		newJob.v1 = v1;
		newJob.v2 = v2;
		newJob.v3 = v3;
		newJob.SubDivisions = aSubDivisions;
		aWorld->PendingJobs.Enqueue(newJob);
	}
}

void ACGWorldFace::UpdateMesh(TArray<FRuntimeMeshVertexSimple>& aVertices, TArray<int32>& aIndices, FCGWorldConfig& aWorldConfig)
{
	MeshComponent->CreateMeshSection(0, aVertices, aIndices, aWorldConfig.CollisionEnabled,  EUpdateFrequency::Infrequent);
	MeshComponent->CookCollisionNow();
}
