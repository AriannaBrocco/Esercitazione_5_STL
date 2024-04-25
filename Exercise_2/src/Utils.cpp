#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
#include <iterator>


namespace PolygonalLibrary {
bool ImportaMesh(const string& nomeFile,
                PolygonalMesh& mesh)
{

    if(!ImportaCell0Ds(nomeFile + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto iter = mesh.Cell0DMarkers.begin(); iter != mesh.Cell0DMarkers.end(); iter++)
        {
            cout << "chiave:\t" << iter -> first << "\t valori:";
            for(const unsigned int id : iter -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportaCell1Ds(nomeFile + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "chiave:\t" << it -> first << "\t valori:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportaCell2Ds(nomeFile + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }

    double tol = numeric_limits<double>::epsilon();

    // Test sui lati
    for(const auto& i : mesh.Cell1DVertices)
    {
        Vector2d coordinata1 = mesh.Cell0DCoordinates[i(0)];
        Vector2d coordinata2 = mesh.Cell0DCoordinates[i(1)];
        if(abs(coordinata1(0)-coordinata2(0)) < tol && abs(coordinata1(1)-coordinata2(1)) < tol)
        {
            cerr << "Alcuni lati hanno lunghezza nulla." << endl;
            return false;
        }
    }

    // Test sull'area, calcolo del prodotto vettoriale di lati successivi (di un poligono di n lati)
    for(const auto& lati : mesh.Cell2DEdges)
    {
        for(unsigned int l = 0; l < lati.size(); l++)
            if(l != lati.size()-1)
            {
                Vector2i puntiLato1 = mesh.Cell1DVertices[lati(l)];
                Vector2d coordVertice1Lato1 = mesh.Cell0DCoordinates[puntiLato1(0)];
                Vector2d coordVertice2Lato1 = mesh.Cell0DCoordinates[puntiLato1(1)];
                Vector2d lunghezzaLato1 = coordVertice2Lato1-coordVertice1Lato1;
                Vector2i puntiLato2 = mesh.Cell1DVertices[lati(l+1)];
                Vector2d coordVertice1Lato2 = mesh.Cell0DCoordinates[puntiLato2(0)];
                Vector2d coordVertice2Lato2 = mesh.Cell0DCoordinates[puntiLato2(1)];
                Vector2d lunghezzaLato2 = coordVertice2Lato2-coordVertice1Lato2;
                double operazione = lunghezzaLato1.x() * lunghezzaLato2.y() - lunghezzaLato1.y() * lunghezzaLato2.x();
                if(abs(operazione) < tol)
                {
                    cerr << "Alcuni poligoni hanno area nulla." << endl;
                    return false;
                }
            }

            // Se sono arrivata all'ultimo lato
            else
            {
                Vector2i puntiLato1 = mesh.Cell1DVertices[lati(l)];
                Vector2d coordVertice1Lato1 = mesh.Cell0DCoordinates[puntiLato1(0)];
                Vector2d coordVertice2Lato1 = mesh.Cell0DCoordinates[puntiLato1(1)];
                Vector2d lunghezzaLato1 = coordVertice2Lato1-coordVertice1Lato1;
                Vector2i puntiLato2 = mesh.Cell1DVertices[lati(0)];
                Vector2d coordVertice1Lato2 = mesh.Cell0DCoordinates[puntiLato2(0)];
                Vector2d coordVertice2Lato2 = mesh.Cell0DCoordinates[puntiLato2(1)];
                Vector2d lunghezzaLato2 = coordVertice2Lato2-coordVertice1Lato2;
                double operazione = lunghezzaLato1.x() * lunghezzaLato2.y() - lunghezzaLato1.y() * lunghezzaLato2.x();
                if(abs(operazione) < tol)
                {
                    cerr << "Alcuni poligoni hanno area nulla." << endl;
                    return false;
                }
            }
    }


    return true;

}
// ***************************************************************************
bool ImportaCell0Ds(const string &nomeFile,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(nomeFile);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string b;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >>  id >> b >> marker >> b >> coord(0) >> b >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {

            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;
}
// ***************************************************************************
bool ImportaCell1Ds(const string &nomeFile,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(nomeFile);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string b;
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> b >> marker >> b >> vertices(0) >> b >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {

            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}
// ***************************************************************************
bool ImportaCell2Ds(const string &nomeFile,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(nomeFile);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DNumVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DNumEdges.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string b;
        unsigned int id;
        unsigned int marker;
        unsigned int NumVertices;
        unsigned int NumEdges;
        VectorXi vertices;
        VectorXi edges;

        converter >> id >> b >> marker >> b >> NumVertices;
        vertices.resize(NumVertices);
        for(unsigned int i = 0; i < NumVertices; i++)
            converter >> b >> vertices(i);
        converter >> b >> NumEdges;
        edges.resize(NumEdges);
        for(unsigned int i = 0; i < NumEdges; i++)
            converter >> b >> edges(i);

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DNumVertices.push_back(NumVertices);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DNumEdges.push_back(NumEdges);
        mesh.Cell2DEdges.push_back(edges);
        if( marker != 0)
        {

            auto ret = mesh.Cell2DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();
    return true;
}

}
