
#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0; /// numero di celle Cell0D
    std::vector<unsigned int> Cell0DId = {}; /// Cell0D id, size 1 x NumberCell0D
    std::vector<Vector2d> Cell0DCoordinates = {}; /// coordinate di Cell0D , size 2 x NumberCell0D (x,y)
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; /// markers di Cell0D , size 1 x NumberCell0D (marker)

    unsigned int NumberCell1D = 0; /// numero di celle Cell1D
    std::vector<unsigned int> Cell1DId = {}; /// Cell1D id, size 1 x NumberCell1D
    std::vector<Vector2i> Cell1DVertices = {}; /// vertici e indici di Cell1D, size 2 x NumberCell1D (fromId,toId)
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; /// proprietà di Cell1D, size 1 x NumberCell1D (marker)

    unsigned int NumberCell2D = 0; /// numero di celle Cell2D
    std::vector<unsigned int> Cell2DId = {}; /// Cell2D id, size 1 x NumberCell2D
    std::vector<unsigned int> Cell2DNumVertices = {};
    std::vector<VectorXi> Cell2DVertices = {}; /// vertici e indici di Cell2D, size 1 x NumberCell2DVertices[NumberCell2D]
    std::vector<unsigned int> Cell2DNumEdges = {};
    std::vector<VectorXi> Cell2DEdges = {}; /// indici di Cell2D Cell1D, size 1 x NumberCell2DEdges[NumberCell2D]
    std::map<unsigned int, list<unsigned int>> Cell2DMarkers = {};
};

}

