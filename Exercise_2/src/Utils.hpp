
#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary {

/// Importa la mesh poligonale (PolygonalMesh è una struttura) e testa se è corretta
/// Ritorna vero se la lettura ha avuto successo, falso altrimenti
/// Anche le altre fanno le stesso
bool ImportaMesh(const string &nomeFile,
                PolygonalMesh& mesh);

/// Importa le proprietà delle celle 0D dal file Cell0Ds.csv
bool ImportaCell0Ds(const string &nomeFile,
                   PolygonalMesh& mesh);

/// Importa le proprietà delle celle 1D dal file Cell1Ds.csv
bool ImportaCell1Ds(const string &nomeFile,
                   PolygonalMesh& mesh);

/// Importa le proprietà delle celle 2D dal file Cell2Ds.csv
bool ImportaCell2Ds(const string &nomeFile,
                   PolygonalMesh& mesh);

}
