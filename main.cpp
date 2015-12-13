#include<stdio.h>
#include<mpi.h> // inkludiere den MPI-Header

int main(int argc, char** argv){
int nodeID, numNodes;

/* Grundfunktionen von MPI: */
MPI_Init(&argc, &argv);
/* Initialisiert das MPI-Laufzeitsystem (Folie), uebergibt MPI die per Kommandozeile uebergebenen Argumente.
Vorher duerfen keine MPI-Funktionen aufgerufen werden. */

MPI_Comm_size(MPI_COMM_WORLD, &numNodes);
/* Gibt Anzahl der Prozesse in Kommunikator MPI_COMM_WORLD zurueck.
Dieser ist der default Kommunikator, und repraesentiert die Menge aller Prozesse. */

MPI_Comm_rank(MPI_COMM_WORLD, &nodeID);
// Gibt die eigene ID (genannt: rank) innerhalb des Kommunikators MPI_COMM_WORLD zurueck.

/* Gibt die eigene ID und die Gesamtanzahl der Prozesse auf der Kommandozeile aus. */
printf("Hello world from process %d of %d\n", nodeID, numNodes);

/* Wartet auf Abschluss aller MPI-Anweisungen und meldet dann Prozess bei MPI-Laufzeitumgebung ab.
Nach MPI_Finalize koennen keine MPI Befehle mehr aufgerufen werden. */
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
