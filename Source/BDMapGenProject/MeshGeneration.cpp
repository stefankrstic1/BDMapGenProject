// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGeneration.h"

FMapMeshVariables UMeshGeneration::GenerateMesh(TArray<TArray<int>> _map, float _squareSize)
{
    SquareSize = _squareSize;

    TriangleDictionary.Empty();
    Outlines.Empty();
    CheckedVertices.Empty();

    SquareGrid = USquareGrid::MAKE();

    Squares = SquareGrid->GenerateSquareGrid(_map, SquareSize);

    for (TArray<USquare*> SquareArray : Squares)
    {
        for (USquare* Square : SquareArray)
        {
            TriangulateSquare(Square);
        }
    }

    CreateWallMesh();

    TArray<FVector2D> UVs;
    for (int i = 0; i < MapMeshVariables.WallVertices.Num(); i++)
    {
        //Maybe not working

        float percentX = UKismetMathLibrary::NormalizeToRange(MapMeshVariables.WallVertices[i].Y , -_map[0].Num() / 2 * _squareSize, _map[0].Num() / 2 * _squareSize);
        float percentY = UKismetMathLibrary::NormalizeToRange(MapMeshVariables.WallVertices[i].X , -_map[0].Num() / 2 * _squareSize, _map[0].Num() / 2 * _squareSize);
        UVs.Add(FVector2D(percentX, percentY));
    }
    MapMeshVariables.WallUVs = UVs;

    return MapMeshVariables;
}

UMeshGeneration* UMeshGeneration::MAKE()
{
    UMeshGeneration* m = NewObject<UMeshGeneration>();
    return m;
}

void UMeshGeneration::TriangulateSquare(USquare* square)
{
    TArray<UVertexNode*> nodes;
    switch (square->Configuration)
    {
    case 0:
        break;

        //1 points:
    case 1:
        nodes.Add(square->CentreLeft);
        nodes.Add(square->CentreBottom);
        nodes.Add(square->BottomLeft);
        MeshFromPoints(nodes);
        break;
    case 2:
        nodes.Add(square->BottomRight);
        nodes.Add(square->CentreBottom);
        nodes.Add(square->CentreRight);
        MeshFromPoints(nodes);
        break;
    case 4:
        nodes.Add(square->TopRight);
        nodes.Add(square->CentreRight);
        nodes.Add(square->CentreTop);
        MeshFromPoints(nodes);
        break;
    case 8:
        nodes.Add(square->TopLeft);
        nodes.Add(square->CentreTop);
        nodes.Add(square->CentreLeft);
        MeshFromPoints(nodes);
        break;

        //2 points:
    case 3:
        nodes.Add(square->CentreRight);
        nodes.Add(square->BottomRight);
        nodes.Add(square->BottomLeft);
        nodes.Add(square->CentreLeft);
        MeshFromPoints(nodes);
        break;
    case 6:
        nodes.Add(square->CentreTop);
        nodes.Add(square->TopRight);
        nodes.Add(square->BottomRight);
        nodes.Add(square->CentreBottom);
        MeshFromPoints(nodes);
        break;
    case 9:
        nodes.Add(square->TopLeft);
        nodes.Add(square->CentreTop);
        nodes.Add(square->CentreBottom);
        nodes.Add(square->BottomLeft);
        MeshFromPoints(nodes);
        break;
    case 12:
        nodes.Add(square->TopLeft);
        nodes.Add(square->TopRight);
        nodes.Add(square->CentreRight);
        nodes.Add(square->CentreLeft);
        MeshFromPoints(nodes);
        break;
    case 5:
        nodes.Add(square->CentreTop);
        nodes.Add(square->TopRight);
        nodes.Add(square->CentreRight);
        nodes.Add(square->CentreBottom);
        nodes.Add(square->BottomLeft);
        nodes.Add(square->CentreLeft);
        MeshFromPoints(nodes);
        break;
    case 10:
        nodes.Add(square->TopLeft);
        nodes.Add(square->CentreTop);
        nodes.Add(square->CentreRight);
        nodes.Add(square->BottomRight);
        nodes.Add(square->CentreBottom);
        nodes.Add(square->CentreLeft);
        MeshFromPoints(nodes);
        break;

        //3 points:
    case 7:
        nodes.Add(square->CentreTop);
        nodes.Add(square->TopRight);
        nodes.Add(square->BottomRight);
        nodes.Add(square->BottomLeft);
        nodes.Add(square->CentreLeft);
        MeshFromPoints(nodes);
        break;
    case 11:
        nodes.Add(square->TopLeft);
        nodes.Add(square->CentreTop);
        nodes.Add(square->CentreRight);
        nodes.Add(square->BottomRight);
        nodes.Add(square->BottomLeft);
        MeshFromPoints(nodes);
        break;
    case 13:
        nodes.Add(square->TopLeft);
        nodes.Add(square->TopRight);
        nodes.Add(square->CentreRight);
        nodes.Add(square->CentreBottom);
        nodes.Add(square->BottomLeft);
        MeshFromPoints(nodes);
        break;
    case 14:
        nodes.Add(square->TopLeft);
        nodes.Add(square->TopRight);
        nodes.Add(square->BottomRight);
        nodes.Add(square->CentreBottom);
        nodes.Add(square->CentreLeft);
        MeshFromPoints(nodes);
        break;

        //4 point:
    case 15:
        nodes.Add(square->TopLeft);
        nodes.Add(square->TopRight);
        nodes.Add(square->BottomRight);
        nodes.Add(square->BottomLeft);

        CheckedVertices.Add(square->TopLeft->VertexIndex);
        CheckedVertices.Add(square->TopRight->VertexIndex);
        CheckedVertices.Add(square->BottomRight->VertexIndex);
        CheckedVertices.Add(square->BottomLeft->VertexIndex);

        MeshFromPoints(nodes);
        break;


    default:
        break;
    }
}

void UMeshGeneration::MeshFromPoints(TArray<UVertexNode*> points)
{
    AssignVertices(points);

    if (points.Num() >= 3)
        CreateTriangle(points[2], points[1], points[0]);
    if (points.Num() >= 4)
        CreateTriangle(points[3], points[2], points[0]);
    if (points.Num() >= 5)
        CreateTriangle(points[4], points[3], points[0]);
    if (points.Num() >= 6)
        CreateTriangle(points[5], points[4], points[0]);
}

void UMeshGeneration::AssignVertices(TArray<UVertexNode*> points)
{
    for (UVertexNode* Node : points)
    {
        if (Node->VertexIndex == -1) {
            Node->VertexIndex = MapMeshVariables.Vertices.Num();
            MapMeshVariables.Vertices.Add(Node->Position);
        }
    }
}

void UMeshGeneration::CreateTriangle(UVertexNode* a, UVertexNode* b, UVertexNode* c)
{
    MapMeshVariables.Triangles.Add(a->VertexIndex);
    MapMeshVariables.Triangles.Add(b->VertexIndex);
    MapMeshVariables.Triangles.Add(c->VertexIndex);

    UTriangleClass* Triangle = UTriangleClass::MAKE(a->VertexIndex, b->VertexIndex, c->VertexIndex);
    AddTriangleToDictionary(Triangle->vertexIndexA, Triangle);
    AddTriangleToDictionary(Triangle->vertexIndexB, Triangle);
    AddTriangleToDictionary(Triangle->vertexIndexC, Triangle);
}

void UMeshGeneration::AddTriangleToDictionary(int vertexIndexKey, UTriangleClass* _Triangle)
{
    if (TriangleDictionary.Contains(vertexIndexKey))
    {
        TriangleDictionary[vertexIndexKey].Add(_Triangle);
    }
    else
    {
        TArray<UTriangleClass*> trianglesList;
        trianglesList.Add(_Triangle);
        TriangleDictionary.Add(vertexIndexKey, trianglesList);
    }
}

bool UMeshGeneration::IsOutlineEdge(int _vertexA, int _vertexB)
{
    TArray<UTriangleClass*> trianglesContainingVertexA = TriangleDictionary[_vertexA];
    int sharedTriangleCount = 0;

    for (int i = 0; i < trianglesContainingVertexA.Num(); i++)
    {
        if (trianglesContainingVertexA[i]->Contains(_vertexB)) {
            sharedTriangleCount++;
            if (sharedTriangleCount > 1) {
                break;
            }
        }
    }

    return sharedTriangleCount == 1;
}

int UMeshGeneration::GetConnectedOutlineVertex(int _vertexIndex)
{
    TArray<UTriangleClass*> trianglesContainingVertex = TriangleDictionary[_vertexIndex];
    for (int i = 0; i < trianglesContainingVertex.Num(); i++)
    {
        UTriangleClass* Triangle = trianglesContainingVertex[i];

        for (int j = 0; j < 3; j++)
        {
            int vertexB = Triangle->vertices[j];
            if (vertexB != _vertexIndex && !CheckedVertices.Contains(vertexB)) {
                if (IsOutlineEdge(_vertexIndex, vertexB)) {
                    return vertexB;
                }
            }
        }
    }

    return -1;
}

void UMeshGeneration::CalculateMeshOutlines()
{
    for (int vertexIndex = 0; vertexIndex < MapMeshVariables.Vertices.Num(); vertexIndex++)
    {
        if (!CheckedVertices.Contains(vertexIndex)) {
            int newOutlineVertex = GetConnectedOutlineVertex(vertexIndex);
            if (newOutlineVertex != -1) {
                CheckedVertices.Add(vertexIndex);
                TArray<int> newOutline;
                newOutline.Add(vertexIndex);
                Outlines.Add(newOutline);
                FollowOutline(newOutlineVertex, Outlines.Num() - 1);
                Outlines[Outlines.Num() - 1].Add(vertexIndex);
            }
        }
    }
}

void UMeshGeneration::FollowOutline(int vertexIndex, int outlineIndex)
{
    Outlines[outlineIndex].Add(vertexIndex);
    CheckedVertices.Add(vertexIndex);
    int nextVertexIndex = GetConnectedOutlineVertex(vertexIndex);

    if (nextVertexIndex != -1) {
        FollowOutline(nextVertexIndex, outlineIndex);
    }
}

void UMeshGeneration::CreateWallMesh()
{
    CalculateMeshOutlines();

    TArray<FVector> wallVertices;
    TArray<int> wallTriangles;
    TArray<FVector2D> wallUVs;

    //DODAJ KAO UPROPERTY da se iz bluprinta moze modifikovati
    float WallHeight = 1000;

    for (TArray<int> outline : Outlines)
    {
        for (int i = 0; i < outline.Num() - 1; i++)
        {
            int startIndex = wallVertices.Num();

            wallVertices.Add(MapMeshVariables.Vertices[outline[i]]); // left
            wallVertices.Add(MapMeshVariables.Vertices[outline[i + 1]]); // right
            wallVertices.Add(MapMeshVariables.Vertices[outline[i]] - FVector(0, 0, 1) * WallHeight); // bottom left
            wallVertices.Add(MapMeshVariables.Vertices[outline[i + 1]] - FVector(0, 0, 1) * WallHeight); // bottom right

            wallTriangles.Add(startIndex + 0);
            wallTriangles.Add(startIndex + 3);
            wallTriangles.Add(startIndex + 2);

            wallTriangles.Add(startIndex + 3);
            wallTriangles.Add(startIndex + 0);
            wallTriangles.Add(startIndex + 1);
        }
    }

    MapMeshVariables.WallTriangles = wallTriangles;
    MapMeshVariables.WallVertices = wallVertices;
    MapMeshVariables.WallUVs = wallUVs;
}
