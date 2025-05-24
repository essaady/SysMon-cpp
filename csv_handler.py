import pandas as pd

def lire_csv(fichier):
    #Lit un fichier CSV et retourne un DataFrame.
    try:
        df = pd.read_csv(fichier)
        print(f"{len(df)} lignes chargées depuis {fichier}")
        return df
    except Exception as e:
        print(f"Erreur lors de la lecture du fichier CSV: {e}")
        return None

def analyser_csv(df):
    #Analyse les colonnes numériques et affiche leurs statistiques.
    if df is not None:
        print("\nStatistiques des colonnes numériques:")
        print(df.describe())
    else:
        print("Aucune donnée à analyser.")

# Exemple d'utilisation
if __name__ == "__main__":
    df = lire_csv("data/exemple.csv")
    analyser_csv(df)
