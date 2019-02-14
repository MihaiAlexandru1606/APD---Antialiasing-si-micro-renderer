---------------------  Algoritmi Paraleli si Distribuiti -----------------------

Tema 1 : Antialiasing si micro renderer
Nume : Niculescu
Prenume : Mihai Alexandru
Grupa : 335CB
Sistem de operare : Ubuntu 18.04 LTS 64bit
Editor : CLion 
Compilator : gcc version 7.3.0 (Ubuntu 7.3.0-16ubuntu3)
Fisiere : homework.c homework1.c homework1.h Readme.txt

--------------------------------------------------------------------------------


<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Structura Temei  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    In fisierul homework.c si homework.h se gasesc: implemnetarea, semnaturile
functilor si structurile pentru imagine si fiecare tip de pixel pentru task 1,
respectiv, in homework1.c si homework1.h, pentru task 2.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  Task 1  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Fișiere: homework.c homework.h main.c
Compilare: gcc -o homework homework.c main.c -lpthread -Wall -lm
Executabil: homework
Rulare: ./homework path_image_input path_image_output resize_factor num_thread

    Task-ul creează o imagine de resize_factor mai mica decât originala. Pentru
acest task am creat o structura specifica pentru fiecare tip de imagine. In
structura image retin tipul de imagine, dimensiunile imaginii si un pointer de
tip void * pentru a oferi mai multa flexibilitate. Matricea de pixeli poate
astfel sa fie atat de tipul PixelGray si PixelColor, dar si matrice normala sau
matrice "liniarizata". Am folosit variabilele globale pentru a fi mai ușor de
folosit de către fiecare thread si pentru am nu mai trebui sa mai creez un
vector de structuri, care trebui sa fie dat fiecărui thread(la fel ca thread_id)
in care singura diferenta ar fi fost decât id respectivului thread.

    Matricea de pixel este alocata sub forma "normala" pentru a facilita
calculele care se vor face in fiecare functie de thread, matricea rezultat va
fi sub forma de matrice liniarizata pentru a economisi memorie si pentru a nu
se apela de mai multe ori functia fwrite. Fiecare matrice de dimensiunea
resize_factor x resize_factor va primi un "id", care va reprezenta indicele
pixelului calculat din noua matrice.
Exemplu: width = height = 4 si resize_factor = 2
    p11 p12 p13 p14
    p21 p22 p23 p24
    p31 p32 p33 p34
    p41 p42 p43 p44

    Submatricea formata din pixeli : p11 p12 p21 p22 va avea id 0, si pe baza ei
se va calcula pixelul cu indicele [0], matrixNew[0] = (p11 + p12 + p21+p22)/4
Submatricea formata din pixeli : p13 p14 p23 p24 va avea id: 1, si pe baza ei se
va calcula pixelul matrixNew[0] = (p13 + p14 + p23 + p24)/4. Submatricea formata
din pixeli : p31 p32 p41 p42 va avea id 2, si pe baza ei se va calcula pixelul
matrixNew[2] = (p31 + p32 + p41 + p42) / 4. Submatricea formata din pixeli : p33
p34 p43 p44, va avea id 3 si pe baza ei se va calcula matrixNew[1] = (p33 + p34
+ p43 + p44) / 4. Pentru a lucra cu cele doua tipuri de reprezentare a unei
matrice, am creat cele doua macro-uri INIT_START_END si INIT_INDEX. Primul este
folosit pentru a calcula start si end, reprezentant indicele de inceput si final
din noua matrice ce urmeaza sa fie calculate de fiecare thread. Un thread va
calcula pixeli din intervalul [start, end] (capetele intervalului sunt calculate
in functie de id-ul thread-ului). Al doilea macro este folosit pentru
determinarea incadrarii submatricei de resize_factor x resize_factor.
    Exista cate o "functie de thread" in functie de tipul de imagine. In cazul
in care resize_factor este 3 se va calcula acea medie ponderata.

    La final este scrisa noua matrice si eliberata memoria alocata pe parcursul
programului.


*) Scalabilitate:
    Timpi optinuti pe cluster-ul facultatii:

+---------------+---------------+----------------+--------------+--------------+
| Imagine Input | Resize Factor | Nr. Thread-uri |     Timp(s)  |  Media(s)    |
+---------------+---------------+----------------+--------------+--------------+
|               |               |                |   0.004891   |              |
|               |               |                |   0.004758   |              |
|               |               |       1        |   0.004727   |   0.004772   |
|               |               |                |   0.004725   |              |
|               |               |                |   0.004758   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.002610   |              |
|               |               |                |   0.002627   |              |
|               |               |       2        |   0.002469   |   0.002545   |
|               |               |                |   0.002493   |              |
|               |               |                |   0.002525   |              |
+               +       2       +----------------+--------------+--------------+
|               |               |                |   0.001380   |              |
|               |               |                |   0.001357   |              |
|               |               |       4        |   0.001366   |   0.001390   |
|               |               |                |   0.001530   |              |
|               |               |                |   0.001318   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.000923   |              |
|               |               |                |   0.000826   |              |
|               |               |       8        |   0.000834   |   0.000856   |
|               |               |                |   0.000847   |              |
|               |               |                |   0.000852   |              |
+               +---------------+----------------+--------------+--------------+
|               |               |                |   0.001863   |              |
|               |               |                |   0.001972   |              |
|               |               |       1        |   0.001960   |   0.001945   |
|               |               |                |   0.001963   |              |
|               |               |                |   0.001966   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.001097   |              |
|               |               |                |   0.001049   |              |
|               |               |       2        |   0.001092   |   0.001092   |
|               |               |                |   0.001109   |              |
|               |               |                |   0.001113   |              |
+ lenna_bw.pgm  +       3       +----------------+--------------+--------------+
|               |               |                |   0.000714   |              |
|               |               |                |   0.000673   |              |
|               |               |       4        |   0.000680   |   0.000689   |
|               |               |                |   0.000686   |              |
|               |               |                |   0.000691   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.000536   |              |
|               |               |                |   0.000540   |              |
|               |               |       8        |   0.000568   |   0.000521   |
|               |               |                |   0.000502   |              |
|               |               |                |   0.000460   |              |
+               +---------------+----------------+--------------+--------------+
|               |               |                |   0.002351   |              |
|               |               |                |   0.002397   |              |
|               |               |       1        |   0.002346   |   0.002380   |
|               |               |                |   0.002405   |              |
|               |               |                |   0.002402   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.001251   |              |
|               |               |                |   0.001293   |              |
|               |               |       2        |   0.001221   |   0.001269   |
|               |               |                |   0.001306   |              |
|               |               |                |   0.001276   |              |
+               +       8       +----------------+--------------+--------------+
|               |               |                |   0.000752   |              |
|               |               |                |   0.000737   |              |
|               |               |       4        |   0.000734   |   0.000740   |
|               |               |                |   0.000748   |              |
|               |               |                |   0.000728   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.000546   |              |
|               |               |                |   0.000580   |              |
|               |               |       8        |   0.000524   |   0.000544   |
|               |               |                |   0.000528   |              |
|               |               |                |   0.000540   |              |
+---------------+---------------+----------------+--------------+--------------+
|               |               |                |   0.007728   |              |
|               |               |                |   0.007769   |              |
|               |               |       1        |   0.007700   |   0.007731   |
|               |               |                |   0.007731   |              |
|               |               |                |   0.007726   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.004002   |              |
|               |               |                |   0.003938   |              |
|               |               |       2        |   0.003946   |   0.003974   |
|               |               |                |   0.004010   |              |
|               |               |                |   0.003974   |              |
+               +       2       +----------------+--------------+--------------+
|               |               |                |   0.002101   |              |
|               |               |                |   0.002110   |              |
|               |               |       4        |   0.002098   |   0.002104   |
|               |               |                |   0.002104   |              |
|               |               |                |   0.002108   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.001383   |              |
|               |               |                |   0.001298   |              |
|               |               |       8        |   0.001219   |   0.001292   |
|               |               |                |   0.001314   |              |
|               |               |                |   0.001246   |              |
+               +---------------+----------------+--------------+--------------+
|               |               |                |   0.004517   |              |
|               |               |                |   0.004559   |              |
|               |               |       1        |   0.004557   |   0.004548   |
|               |               |                |   0.004542   |              |
|               |               |                |   0.004564   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.002317   |              |
|               |               |                |   0.002392   |              |
|               |               |       2        |   0.002405   |   0.002383   |
|               |               |                |   0.002393   |              |
|               |               |                |   0.002409   |              |
+lenna_color.pnm+       3       +----------------+--------------+--------------+
|               |               |                |   0.001341   |              |
|               |               |                |   0.001325   |              |
|               |               |       4        |   0.001309   |   0.001318   |
|               |               |                |   0.001338   |              |
|               |               |                |   0.001277   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.000878   |              |
|               |               |                |   0.000893   |              |
|               |               |       8        |   0.000837   |   0.000865   |
|               |               |                |   0.000886   |              |
|               |               |                |   0.000833   |              |
+               +---------------+----------------+--------------+--------------+
|               |               |                |   0.004221   |              |
|               |               |                |   0.004275   |              |
|               |               |       1        |   0.004275   |   0.004272   |
|               |               |                |   0.004275   |              |
|               |               |                |   0.004312   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.002213   |              |
|               |               |                |   0.002260   |              |
|               |               |       2        |   0.002244   |   0.002237   |
|               |               |                |   0.002258   |              |
|               |               |                |   0.002210   |              |
+               +       8       +----------------+--------------+--------------+
|               |               |                |   0.001215   |              |
|               |               |                |   0.001269   |              |
|               |               |       4        |   0.001235   |   0.001237   |
|               |               |                |   0.001238   |              |
|               |               |                |   0.001227   |              |
+               +               +----------------+--------------+--------------+
|               |               |                |   0.000824   |              |
|               |               |                |   0.000786   |              |
|               |               |       8        |   0.000784   |   0.000804   |
|               |               |                |   0.000784   |              |
|               |               |                |   0.000840   |              |
+---------------+---------------+----------------+--------------+--------------+

    Din datele optinute se poate obseva cum timpul pentru functia resize scade
aproape proportional cu cat cresc numarul de thread-uri (exista operationea de
alocare a memorie care este costisitoare si va dura la fel indiferent de numarul
de thread-uri).

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<     Task 2      >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Fisiere: homework1.c homework1.h main1.c
Compilare: gcc -o homework1 homework1.c main1.c -lpthread -Wall -lm
Executabil: homework1
Rulare: ./homework1 path_image_output resolution num_thread

    Programul creaza o imagine in care va gasi o linie neagra...
    Pentru acest task am lucrat cu o matrice liniarizata, pentru a economisi
memoria. Structura auxiliara folosita este PixelGray.

    Un thread va calula pixeli din intervalul [start, end], care sunt la fel
calculati ca la task-ul 1. Apoi sunt calculati indicii in varianta in care
matricea este sub o "normala". Pe baza acestor indici sunt calculate
coordonatele "mijlocului pixelului" din planul xOy, in acelasi timp scaland la
dimensiunile de 100 cm x 100 cm. Este calculata distanta pana la dreapta si daca
este mai mica decât 3, atunci este negru, altfel este alb.

    La final este scrisa imaginea si dealocata memoria.

*) Scalabilitate:
    Timpi optinuti pe cluster-ul facultatii

+------------+----------------+-----------------------+------------------------+
| Resolution | Nr. Thread-uri |        Timp(s)        |        Media(s)        |
+------------+----------------+-----------------------+------------------------+
|            |                |        0.075362       |                        |
|            |                |        0.076714       |                        |
|            |        1       |        0.077038       |        0.075612        |
|            |                |        0.073139       |                        |
|            |                |        0.075805       |                        |
+            +----------------+-----------------------+------------------------+
|            |                |        0.041234       |                        |
|            |                |        0.041330       |                        |
|            |        2       |        0.039371       |        0.040366        |
|            |                |        0.039660       |                        |
|            |                |        0.040236       |                        |
+    1000    +----------------+-----------------------+------------------------+
|            |                |        0.018566       |                        |
|            |                |        0.017436       |                        |
|            |        4       |        0.017055       |        0.017595        |
|            |                |        0.017086       |                        |
|            |                |        0.017833       |                        |
+            +----------------+-----------------------+------------------------+
|            |                |        0.008137       |                        |
|            |                |        0.008126       |                        |
|            |        8       |        0.008760       |        0.008542        |
|            |                |        0.008061       |                        |
|            |                |        0.009628       |                        |
+------------+----------------+-----------------------+------------------------+
|            |                |        2.945464       |                        |
|            |                |        2.893259       |                        |
|            |        1       |        2.942595       |        2.917793        |
|            |                |        2.896953       |                        |
|            |                |        2.910692       |                        |
+            +----------------+-----------------------+------------------------+
|            |                |        1.498699       |                        |
|            |                |        1.499516       |                        |
|            |        2       |        1.447580       |        1.479072        |
|            |                |        1.450021       |                        |
|            |                |        1.499545       |                        |
+    8000    +----------------+-----------------------+------------------------+
|            |                |        0.746890       |                        |
|            |                |        0.757234       |                        |
|            |        4       |        0.752968       |        0.746514        |
|            |                |        0.725239       |                        |
|            |                |        0.750241       |                        |
+            +----------------+-----------------------+------------------------+
|            |                |        0.368433       |                        |
|            |                |        0.364739       |                        |
|            |        8       |        0.364508       |        0.370574        |
|            |                |        0.364016       |                        |
|            |                |        0.391174       |                        |
+------------+----------------+-----------------------+------------------------+

    Din datele optinute se poate obseva cum timpul pentru functia render 
scade aproape proportional cu cat cresc numarul de thread-uri (exista operationea
de alocare a memorie care este costisitoare si va dura la fel indiferent de 
numarul de thread-uri).

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
