/**************** Projet PNL ****************/ 
/////////// Invite de commande pour le noyau Linux ///////////
_____________________________________________________
|---------- Nom: Mahamat Issa Abderahim ------------|
|---------- Numero etudiant : 3362755         ------|
|---------- @issathink - github.com/issathink ------|
|__________________ ________________________________|               
Structure du Projet :

Projet
├── common :                                    repertoire contenant le fichier header a partager entre le module et le client
│   └── ioctl_infos.h
├── Makefile                                    Makefile general
├── module                                      code source du module
│   ├── commande_fg.c
│   ├── commande_kill.c
│   ├── commande_list.c
│   ├── commande_meminfo.c
│   ├── commande_modinfo.c
│   ├── commandes.c
│   ├── commandes.h
│   ├── commande_wait.c
│   ├── ioctl_infos.h
│   ├── Makefile
├── README.txt
├── swapfile.patch.xz                           patch a appliquer au noyau
├── test_client                                 repertoire de l'outil pour tester le module
│   ├── exec
│   ├── ioctl_infos.h
│   ├── Makefile
│   ├── test.c
│   ├── tools.c
│   ├── tools.h
└── 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************** I - Mode d'emploi *************************/
Pre-requis:
        * Noyau Linux compilé (version >= 4.2.3, idealement 4.2.3)  
        * Avoir une version recente de QEMU
        
1. Telecharger et decompresser les sources
2. Faire (a la racine de votre noyau compile) en indiquant le chemin du fichier de patch :
        xzcat ~/votrehome/Projet/swapfile.patch.xz | patch -p1
3. Recompiler le noyau.
4. Placez vous a la racine du projet (dossier Projet)
5. Puis faire :
        make KERNELDIR=/chemin/vers/racine/noyau/compile
6. Lancer  la machine virtuelle :
        ./qemu-run-externKernel.sh
7. Copier les fichiers mycmd.ko et exec  sur le machine virtuelle :
        scp yourusername@10.0.2.2:/yourhome/Projet/module/mycmd.ko yourusername@10.0.2.2:/yourhome/Projet/test_client/exec .
8. Inserer le module :
        insmod mycmd.ko
9. Creer le fichier special pour le  ioctl :
        mknod /dev/commandes_ioctl  c  numero_affiche_sur_la_console  0
10. Lancer l'outil :
        ./exec

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************** II - Traces du projet ************************/
Tout ce qui a ete demande pour le projet a ete fait. Voici la trace de quelques tests :

1. Commande KILL:      
        KILL 9 309 &
        
2. La sortie de la commande LIST:
        LIST
        List (ids) of running processes:
        3
        
3. commande FG:
        FG 3
        FG : Successfuly killed process pid: 309      

4. Modinfo :
        MODINFO mycmd
        modinfo name: mycmd.ko
        Name: mycmd
        Version: 1.0
        Args:
        Load adress: ffffffffa00018c0 
        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************** III - Explications *************************/
Ce qui a ete fait:
        - fonctions synchrones : LIST, FG, KILL, WAIT, MEMINFO, MODINFO
        - fonctions asynchrones: KILL, MEMINFO, MODINFO

Il a ete decide que faire un wait en background n'a pas trop d'interet.
Le but de la commande LIST est d'afficher la liste (des identifiants) des processus en cours d'exection pour faire FG dessus donc il est pas interessant (choix) de le lancer en background.

