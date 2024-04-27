#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>


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
        cout << endl;
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
        cout << endl;
    }

    if(!ImportaCell2Ds(nomeFile + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    cout << "Corretta importazione delle celle 2D" << endl << endl;

    double tol = numeric_limits<double>::epsilon();

    // Test sui lati
    cout << "Test sui lati: " << endl;
    for(const auto& i : mesh.Cell1DVertices)
    {
        Vector2d coordinata1 = mesh.Cell0DCoordinates[i(0)];
        Vector2d coordinata2 = mesh.Cell0DCoordinates[i(1)];
        if(abs(coordinata1(0)-coordinata2(0)) < tol && abs(coordinata1(1)-coordinata2(1)) < tol)
        {
            cerr << "Alcuni lati hanno lunghezza nulla" << endl;
            return false;
        }
    }
    cout << "Nessun lato ha lunghezza nulla" << endl << endl;
    cout << "Test sulle aree:" << endl;

    for(const auto& lati : mesh.Cell2DEdges)
    {
        Vector2i puntiLato = mesh.Cell1DVertices[lati(0)];
        Vector2d coordVertice_1 = mesh.Cell0DCoordinates[puntiLato(0)];
        Vector2d coordVertice_2 = mesh.Cell0DCoordinates[puntiLato(1)];

        // Calcola la pendenza e l'intercetta della retta passante per i due vertici del lato
        double coef_ang = (coordVertice_2[1]-coordVertice_1[1])/(coordVertice_2[0]-coordVertice_1[0]);
        double intercetta = coordVertice_2[0] - (coef_ang * coordVertice_1[0]);

        bool su_stessa_retta = true;
        size_t num_lati = lati.size();
        for(size_t n = 2; n < num_lati; ++n)
        {
            Vector2i puntiLato_n = mesh.Cell1DVertices[lati(n)];
            Vector2d coordVertice_n = mesh.Cell0DCoordinates[puntiLato_n(0)];
            double y = coef_ang * coordVertice_n[0] + intercetta;

            if (coordVertice_n[1] != y)
            {
                su_stessa_retta = false;
                break;
            }
        }

        if (su_stessa_retta)
        {
            cout << "Ci sono poligoni con area nulla" << endl;
        }

    }

    cout << "Non ci sono poligoni con area nulla" << endl;


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
        cerr << "Non ci sono celle 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        char sep;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >>  id >> sep >> marker >> sep >> coord(0) >> sep >> coord(1);

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
        cerr << "Non ci sono celle 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        char sep;
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> sep >> marker >> sep >> vertices(0) >> sep >> vertices(1);

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
        cerr << "Non ci sono celle 2D" << endl;
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
        char sep;
        unsigned int id;
        unsigned int marker;
        unsigned int NumVertices;
        unsigned int NumEdges;
        VectorXi vertices;
        VectorXi edges;

        converter >> id >> sep >> marker >> sep >> NumVertices;
        vertices.resize(NumVertices);
        for(unsigned int i = 0; i < NumVertices; i++)
            converter >> sep >> vertices(i);

        converter >> sep >> NumEdges;
        edges.resize(NumEdges);
        for(unsigned int i = 0; i < NumEdges; i++)
            converter >> sep >> edges(i);

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
