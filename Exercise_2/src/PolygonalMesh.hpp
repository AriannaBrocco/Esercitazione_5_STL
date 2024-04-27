
#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0; /// numero di celle Cell0D
    vector<unsigned int> Cell0DId = {}; /// Cell0D id
    vector<Vector2d> Cell0DCoordinates = {}; /// coordinate di Cell0D
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; /// markers di Cell0D

    unsigned int NumberCell1D = 0; /// numero di celle Cell1D
    vector<unsigned int> Cell1DId = {}; /// Cell1D id
    vector<Vector2i> Cell1DVertices = {}; /// vertici e indici di Cell1D
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; /// proprietà di Cell1D

    unsigned int NumberCell2D = 0; /// numero di celle Cell2D
    vector<unsigned int> Cell2DId = {}; /// Cell2D id, size 1 x NumberCell2D
    vector<unsigned int> Cell2DNumVertices = {};
    vector<VectorXi> Cell2DVertices = {}; /// vertici e indici di Cell2D
    vector<unsigned int> Cell2DNumEdges = {};
    vector<VectorXi> Cell2DEdges = {}; /// indici di Cell2D Cell1D
    map<unsigned int, list<unsigned int>> Cell2DMarkers = {};
};

}

