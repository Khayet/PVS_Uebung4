#include<stdio.h>
#include<mpi.h> // inkludiere den MPI-Header

int main(int argc, char** argv){
int nodeID, numNodes;

/* Grundfunktionen von MPI: */
MPI_Init(&argc, &argv);
/* Initialisiert das MPI-Laufzeitsystem (Folie), uebergibt MPI die per Kommandozeile uebergebenen Argumente.
Vorher duerfen keine MPI-Funktionen aufgerufen werden. */
MPI_Comm_size(MPI_COMM_WORLD, &numNodes); // Gibt die Anzahl aller Prozesse im Kommunikator 'MPI_COMM_WORLD' zurueck.
MPI_Comm_rank(MPI_COMM_WORLD, &nodeID); // Gibt die Nummer des ausfuehrenden Prozesses im Kommunikator 'MPI_COMM_WORLD' zurueck.

/* ??? */
printf("Hello world from process %d of %d\n", nodeID, numNodes);

/* ??? */
MPI_Finalize();

return 0;
}


/*  Beispielausfuehrung (bash shell):
 *  alias run='/opt/openmpi/bin/mpirun -np 4 main'
 *  run
 *  Hello world from process 2 of 4
 *  Hello world from process 0 of 4
 *  Hello world from process 1 of 4
 *  Hello world from process 3 of 4
*/
