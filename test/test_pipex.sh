#!/bin/bash

# Chemin vers ton exécutable pipex
PIPEX="./pipex"

# Création de fichiers temporaires
INPUT="input.txt"
EXPECTED="expected_output.txt"
OUTPUT="output.txt"
LOG="test_results.log"

# Efface les résultats précédents
echo "Résultats des tests pour Pipex" > $LOG

# Création d'un fichier d'entrée de base
echo -e "Hello World\nThis is a test file\nHello Pipex\nAnother line with Pipex" > $INPUT

# Fonction pour comparer la sortie de pipex avec la commande shell
function run_test() {
    local cmd1="$1"
    local cmd2="$2"
    local description="$3"

    # Commande de référence avec le shell
    bash -c "< $INPUT $cmd1 | $cmd2 > $EXPECTED"
    echo -e "\nRETURN VALUE: $?" >> $EXPECTED

    # Exécution de Pipex
    $PIPEX $INPUT "$cmd1" "$cmd2" $OUTPUT
    echo -e "\nRETURN VALUE: $?" >> $OUTPUT

    # Comparaison des résultats
    if diff -q $EXPECTED $OUTPUT > /dev/null 2>&1; then
        echo "✔️  Test réussi : $description" >> $LOG
    else
        echo "--------------------------------------" >> $LOG
        echo "❌ Test échoué : $description" >> $LOG
        echo "Résultat attendu :" >> $LOG
        cat $EXPECTED >> $LOG
        echo "Résultat obtenu :" >> $LOG
        cat $OUTPUT >> $LOG
        echo "--------------------------------------" >> $LOG
    fi

    # Nettoyage des fichiers
    rm -f $EXPECTED $OUTPUT
}

# Tests de base
run_test "cat" "wc -l" "cat | wc -l"                    # Compter les lignes
run_test "grep 'Pipex'" "wc -l" "grep Pipex | wc -l"       # Compter les lignes avec un motif
run_test "awk '{print \$1}'" "sort" "awk '{print \$1}' | sort"  # Première colonne triée
run_test "sort" "uniq" "sort | uniq"                      # Trier puis éliminer les doublons
run_test "cut -d ' ' -f 1" "sort" "cut -d ' ' -f 1 | sort"  # Extraire la première colonne puis trier
run_test "cat" "grep 'Pipex'" "cat | grep Pipex"           # Cat puis grep sur un motif spécifique
run_test "cat" "sort" "cat | sort"                         # Trier les lignes du fichier d'entrée

# Tests avec redirection de fichiers (ajustés)
echo -e "Hello World\nAnother line" > $INPUT
run_test "cat" "sort" "cat input.txt | sort"               # Trier un fichier
run_test "grep Hello" "wc -l" "grep Hello input.txt | wc -l" # Compter les lignes contenant 'Hello'

# Tests de transformation de texte
run_test "awk '{print \$2}'" "sort" "awk '{print \$2}' | sort"  # Extraire la 2ème colonne puis trier
run_test "sed 's/Hello/Hi/'" "awk '/Hi/'" "sed 's/Hello/Hi/' | awk '/Hi/'" # Remplacer puis filtrer
run_test "sed 's/Hello/Hi/'" "grep 'Hi'" "sed 's/Hello/Hi/' | grep 'Hi'" # Remplacer puis chercher 'Hi'
run_test "cut -d ' ' -f 2" "sort" "cut -d ' ' -f 2 | sort" # Extraire 2ème colonne puis trier

# Tests avec des erreurs
run_test "wrongcommand" "wc -l" "Test avec commande invalide (1ère)"  # Commande invalide
run_test "cat" "wrongcommand" "Test avec commande invalide (2ème)"    # Deuxième commande invalide
run_test "grep 'NotInFile'" "wc -l" "Test grep sur une chaîne inexistante" # Mot inexistant avec grep
run_test "cat" "grep 'Pipex'" "Test avec grep sur un motif présent"  # Test grep sur un motif présent
run_test "cat" "grep 'notfound'" "Test grep sur un motif inexistant" # Test grep sur un motif inexistant

# Tests avec des fichiers temporaires
echo -e "Line 1\nLine 2\nLine 3" > $INPUT
run_test "cat" "grep 2" "cat input.txt | grep 2"   # Filtrer '2' dans le fichier
run_test "cat" "grep 3" "cat input.txt | grep 3"   # Filtrer '3' dans le fichier
run_test "cat input.txt" "sort" "cat input.txt | sort"   # Trier le fichier

# Tests avec des expressions régulières et des motifs
run_test "grep 'Hello'" "wc -l" "grep 'Hello' | wc -l"  # Compter les occurrences de 'Hello'
run_test "grep -o 'Hello'" "wc -l" "grep -o 'Hello' | wc -l"  # Compter les occurrences exactes de 'Hello'
run_test "grep '^H'" "wc -l" "grep '^H' | wc -l"          # Lignes commençant par 'H'
run_test "grep 'Pipex'" "wc -l" "grep 'Pipex' | wc -l"    # Vérifier le nombre de lignes contenant 'Pipex'

# Tests avec des chaînes spécifiques
run_test "cat" "grep 'Another'" "cat input.txt | grep 'Another'"  # Recherche d'une chaîne spécifique
run_test "cat" "grep 'This'" "cat input.txt | grep 'This'"        # Recherche d'une autre chaîne
run_test "cat" "grep 'World'" "cat input.txt | grep 'World'"      # Recherche d'une chaîne existante
run_test "cat" "grep 'notfound'" "cat input.txt | grep 'notfound'" # Recherche d'une chaîne inexistante

# Tests de plusieurs transformations
run_test "echo 'Hello World'" "sed 's/Hello/Hi/'" "echo 'Hello World' | sed 's/Hello/Hi/'" # Remplacer 'Hello' par 'Hi'
run_test "echo 'test string'" "awk '{print \$2}'" "echo 'test string' | awk '{print \$2}'" # Extraire le second mot

# Tests avec de très longues chaînes
run_test "echo 'A very long string that should not be repeated over and over again to test the efficiency of the program.'" "wc -l" "echo 'A very long string that should not be repeated over and over again to test the efficiency of the program.' | wc -l" # Long string

# Tests de gestion de l'ordre d'exécution
run_test "echo 'one line'" "cat" "echo 'one line' | cat" # Tester l'ordre d'exécution : echo puis cat

# Tests de performance (grande taille de fichier)
yes "Line of text" | head -n 1000 > $INPUT
run_test "cat" "wc -l" "cat input.txt | wc -l"  # Compter les lignes d'un fichier avec 1000 lignes

# Affichage des résultats
echo -e "\n======================== Résultats des tests ========================\n"
cat $LOG

# Nettoyage
rm -f $INPUT $EXPECTED $OUTPUT
