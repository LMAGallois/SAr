Placer les fichiers de données des fenetres de visibilité dans le dossier data

Dans in_out/config.txt, instancier les paramètres decrivant la constellation de satellites considérée.
Attention, le nombre de variables générées risque de faire planter l'ordinateur.
Après tout changement des paramètres de ce fichier, faire un make clean.

Compilation et linkage : make 
Execution : ./sched_Cband

Après execution, plusieurs fichiers sont générés. 
Dans in_out/contacts.txt, on retrouve l'échelle de temps considérée. 
Dans in_out/plan.txt, on retrouve le planning des affectations des satellites généré par le solveur.

Deux modèles à nombres entiers peuvent être executés.
Dans main.cpp, on fait appel à la fonction solveMIP_basic() ou solveMIP() (à commenter et décommenter si besoin).

LEGENDE

indices :
    a : antenne
    sat : satellite
    c : contact (slot de temps)
    u : utilisateur
    e : fin du contact c
    s : début du contact c

données d'entrée : 
    nv_a(a,sat,c) : non visibilité d'une antenne avec un satellite au contact c donné
    nv_u(u,sat,c) : non visibilité d'un utilisateur avec un satellite au contact c donné

variables :

    x(u,sat,c) : connexion entre un utilisateur et un satellite lors du contact c
    g(a,sat,c) : connexion entre une antenne et un satellite lors du contact c
    z(u,c) : nombre de fois qu'un utilisateur n'est pas couvert par au moins deux satellites (contrainte SAR)

1ER MODELE

    spécifications des variables:
        //x vaut soit 0 soit 1
        //g vaut soit 0 soit 1
        //x(u,sat,c) = 0 si nv_u(u,sat,c) = 1
        //g(a,sat,c) = 0 si nv_a(a,sat,c) = 1
    contraintes :
        //somme sur a de g(a,sat,c) >= x(u,sat,c) pour tout (u, sat, c)
        //somme sur sat de g(a,sat,c) <= 1 pour tout (a,c)
        //somme sur sat de x(u,sat,c) <= 2 pour tout (u,c)
    fonction objectif :
        //max de somme sur sat sur c et sur u de x(u,s,c) * (e - s)

2E MODELE

    spécifications des variables:
        //x vaut soit 0 soit 1
        //g vaut soit 0 soit 1
        //z est un entier positif 
        //x(u,sat,c) = 0 si nv_u(u,sat,c) = 1
        //g(a,sat,c) = 0 si nv_a(a,sat,c) = 1
    contraintes :
        //somme sur a de g(a,sat,c) >= x(u,sat,c) pour tout (u, sat, c)
        //somme sur sat de g(a,sat,c) <= 1 pour tout (a,c)
        //somme sur sat de x(u,sat,c) >= 2 - z(u,c) pour tout (u,c)
    fonction objectif :
        //min de somme sur u et sur c de z(u,c)

3E MODELE
[...]
