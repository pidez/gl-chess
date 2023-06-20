
/* Questa è la classe che dovrebbe gestire la scena nella sua interezza.
    Tutti gli oggetti che hanno in qualche modo effetto sulla scena devono essere gestiti tramite questa classe.
    Ad esempio la camera a cui si fa riferimento per il movimento deve essere la camera definita qui. (di conseguenza la
    posizione di camera passata agli shader sarà quella della camera contenuta in questa classe) */

    /* Quando si esegue il rendering:
    
        1. le info sul materiale e sulla trasformazioni di modello sono
            gestite dalla classe Mesh, in particolare nella chiamata a draw(Program)
        2.  le info riguardo la trasformazione di camera e la trasformazione proiettiva sono
            gestite dall'oggetto camera membro di questa classe
        3.  Resta da progettare una classe per gestire le info sulla luce */

#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Program.h"
#include "Camera.h"

class Scene {

public:

    Scene();
    Scene(std::vector<Mesh>& meshes, std::vector<Program>& programs);

    void setCamera(const Camera& camera);

    /* Si attiva la funzionalità di picking degli oggetti, dunque
    ogni operazione di rendering verrà eseguita due volte: una volta utilizzando
    un colore univoco per ogni mesh, l'altra con i colori originali. */
    void enablePicking();
    void disablePicking();

    bool pickingEnabled();



    //Funzione che riceve il punto selezionato dal mouse e gestisce la logica di picking
    void mousePicking(int x, int y);

    /* Disegna tutte le mesh */
    void draw();



    /* Disegna le mesh specificate dal vettore di indici */
    void drawMeshes(std::vector<unsigned int> meshIndices) const;

    void translateSelected(glm::vec3 t);
    void rotateSelected(glm::vec3 axe, float angle);
    
    //generica trasformazione
    void setSelectionModelTransform(glm::mat4& t);

    Camera& camera();




private:

    Camera _camera; ///< La camera posizionata nella scena

    /* Ogni Mesh è identificata dalla posizione all'interno dell'array */
    std::vector<Mesh> _meshes; ///< elenco delle mesh della scena
    
    std::vector<Program> _programs; ///< programmi di shader associati alle rispettive mesh (in ordine)

    /* Array utilizzato solo in caso di attivazione della funzionalità di picking */
    std::vector<glm::vec3> _colors; ///< colori univoci per ogni mesh

    bool _picking_enabled; ///< 
    int _picked_object; ///< ID dell'oggetto attualmente selezionato (Se ne esiste uno, valore negativo altrimenti)


    //Vengono aggiornate dopo ogni rendering, per gestire eventuale resize
    unsigned int _last_x;
    unsigned int _last_y;
    
    bool _interacted; // falso finche non avviene la prima interazione con la scnea

    void drawNoPicking();
    void drawPicking();
};

#endif