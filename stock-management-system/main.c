#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HT_SIZE 1301
#define HELPH_SIZE 2609
#define DELETED_ENTRY ((char)1)

typedef struct {
	char date[11]; // dd/mm/yyyy
	float close;
	int volume;
	float open;
	float high;
	float low;
} Day;

typedef struct {
	char name[32];
	char wkn[32];
	char krzl[32];
	Day history[30];
} Aktie;

typedef struct {
	char key[32]; //Name oder Kürzel
	int idx; //Index des Eintrags Haupttabelle, -1 leer, -2 gelöscht
} Tindex;

void trimNewline();
int isDeletedAktie();
int isEmptyAktie();
int hash();
int getIndex();
void updateHelpH();
void deleteHelpHEntry();
Aktie* search();
Aktie* checkInput();
void add();
void del();
void importData();
void getKurs();
void plot();
void deleteHt();
void deleteHelpHt();
void save();
void load();

int main() {
	Aktie ht[HT_SIZE];
	Tindex helpH[HELPH_SIZE];

	deleteHt(ht);
	deleteHelpHt(helpH);

	while (1) {
		printf("Optionen:\n ADD (a)\n DEL (d)\n IMPORT (i)\n SEARCH (s)\n PLOT (p)\n SAVE (e)\n LOAD (l)\n QUIT (q)\n");

		char c[16];
		fgets(c, sizeof(c), stdin);

		switch (c[0]) {
			case 'a':
				add(ht, helpH);
				break;
			case 'd':
				del(ht, helpH);
				break;
			case 'i':
				importData(ht, helpH);
				break;
			case 's':
				getKurs(ht, helpH);
				break;
			case 'p':
				plot(ht, helpH);
				break;
			case 'e':
				save(ht);
				break;
			case 'l':
				load(ht, helpH);
				break;
			case 'q':
				deleteHt(ht);
				deleteHelpHt(helpH);
				return 0;
			default:
				printf("\nFalsche Eingabe!\n");
		}
		printf("\n");
	}
}

void trimNewline(char *s) { //entfernt \n beim einlesen mit fget
	s[strcspn(s, "\n")] = '\0';
}

int isDeletedAktie(const Aktie* a) {
	return a->name[0] == DELETED_ENTRY;
}

int isEmptyAktie(const Aktie* a) {
	return a->name[0] == '\0';
}

// bei help == 0 wird für ht gehashed bei help == 1 für helpH
int hash(char *str, int help) {
	int h = 0, mod = (help == 0) ? HT_SIZE : HELPH_SIZE;
	while (*str) {
		h = (h * 31 + (unsigned char)(*str)) % mod;
		str++;
	}
	return h;
}

// liefert idx aus helpH oder -1
int getIndex(Tindex* helpH, char* key) {
	int i = 0;
	while (i < HELPH_SIZE) {
		Tindex *entry = &helpH[(hash(key, 1) + i * i) % HELPH_SIZE];
		if (entry->idx >= 0 && strcmp(entry->key, key) == 0) return entry->idx;
		if (entry->idx == -1) break;
		i++;
	}
	return -1;
}

//speichert idx von Eintrag 2 mal in helP (wird 2 mal aufgerufen, einmal mit name und einmal mit krzl)
void updateHelpH(Tindex* helpH, char* key, int idx) {
	int i = 0;
	while (i < HELPH_SIZE) {
		int id = (hash(key, 1) + i * i) % HELPH_SIZE;

		if (helpH[id].idx == -1 || helpH[id].idx == -2 || strcmp(helpH[id].key, key) == 0) { //wenn -1 leer, -2 gelöscht oder der gleiche key
			strcpy(helpH[id].key, key);
			helpH[id].idx = idx;
			return;
		}
		i++;
	}
}

//löscht den Eintrag des keys
void deleteHelpHEntry(Tindex* helpH, char* key) {
	int i = 0;
	while (i < HELPH_SIZE) {
		int id = (hash(key, 1) + i * i) % HELPH_SIZE;
		if (helpH[id].idx == -1) return; //wir landen auf leerem Feld, key nicht gefunden
		if (helpH[id].idx >= 0 && strcmp(helpH[id].key, key) == 0) {
			helpH[id].key[0] = '\0';
			helpH[id].idx = -2; //als gelöscht markieren
			return;
		}
		i++;
	}
}

// direkte Suche über exakten Index aus helpH
Aktie* search(Aktie* ht, int index, char* key) {
	if (index < 0 || index >= HT_SIZE) return NULL;
	if (isEmptyAktie(&ht[index]) || isDeletedAktie(&ht[index])) return NULL;

	if (strcmp(ht[index].name, key) == 0 || strcmp(ht[index].krzl, key) == 0) { //key kann name oder krzl sein
		return &ht[index];
	}
	return NULL;
}

Aktie* checkInput(Aktie* ht, Tindex* helpH) {
	char input[32];
	fgets(input, sizeof(input), stdin);
	trimNewline(input);

	int index = getIndex(helpH, input);
	Aktie* ap = search(ht, index, input);
	if (ap == NULL) {
		printf("\nAktie nicht vorhanden!\n");
	}
	return ap;
}

void add(Aktie* ht, Tindex* helpH) {
	Aktie temp = {0};

	printf("Name: ");
	fgets(temp.name, sizeof(temp.name), stdin);
	trimNewline(temp.name);

	printf("WKN: ");
	fgets(temp.wkn, sizeof(temp.wkn), stdin);
	trimNewline(temp.wkn);

	printf("Kürzel: ");
	fgets(temp.krzl, sizeof(temp.krzl), stdin);
	trimNewline(temp.krzl);

	if (temp.name[0] == '\0' || temp.krzl[0] == '\0' || temp.wkn[0] == '\0') {
		printf("\nUngültige Eingabe!");
		return;
	}

	if (getIndex(helpH, temp.name) != -1 || getIndex(helpH, temp.krzl) != -1) {
		printf("\nAktie mit diesem Namen oder Kürzel existiert bereits!");
		return;
	}

	for (int j = 0; j < 30; j++) {
		temp.history[j] = (Day){0};
	}

	int i = 0;
	while (i < HT_SIZE) {
		int id = (hash(temp.name, 0) + i * i) % HT_SIZE;
		if (isEmptyAktie(&ht[id]) || isDeletedAktie(&ht[id])) {
			ht[id] = temp;
			updateHelpH(helpH, ht[id].name, id);
			updateHelpH(helpH, ht[id].krzl, id);
			printf("\n%s wurde erfolgreich hinzugefügt!", temp.name);
			return;
		}
		i++;
	}

	printf("\nHashtabelle ist voll!");
}

void del(Aktie* ht, Tindex* helpH) {
	printf("\nWelche Aktie soll gelöscht werden? (name/kürzel): ");

	Aktie* ap = checkInput(ht, helpH);
	if (!ap) return;

	char deletedName[32];
	strcpy(deletedName, ap->name);

	deleteHelpHEntry(helpH, ap->name);
	deleteHelpHEntry(helpH, ap->krzl);

	ap->name[0] = DELETED_ENTRY;
	ap->name[1] = '\0';
	ap->krzl[0] = '\0';
	ap->wkn[0] = '\0';

	for (int i = 0; i < 30; i++) {
		ap->history[i] = (Day){0};
	}

	printf("\nAktie %s wurde erfolgreich gelöscht", deletedName);
}

void importData(Aktie* ht, Tindex* helpH) {
	printf("\nZu welcher Aktie sollen Daten importiert werden? \n(Name/Kürzel:) ");

	Aktie* ap = checkInput(ht, helpH);
	if (!ap) return;

	printf("\nAus welchem File soll importiert werden? (Dateipfad:) ");
	char pfad[1000];
	fgets(pfad, sizeof(pfad), stdin);
	trimNewline(pfad);

	FILE* file = fopen(pfad, "r");
	if (file == NULL) {
		printf("\nFile konnte nicht geöffnet werden");
		return;
	}

	char line[256];
	fgets(line, sizeof(line), file); // Header überspringen

	int i = 0;
	while (
		i < 30 &&
		fscanf(file, "%10[^,],$%f,%d,$%f,$%f,$%f\n",
		       ap->history[i].date,
		       &ap->history[i].close,
		       &ap->history[i].volume,
		       &ap->history[i].open,
		       &ap->history[i].high,
		       &ap->history[i].low) == 6
	) {
		i++;
	}

	fclose(file);
	printf("\n%d Kurseinträge importiert.", i);
}

void getKurs(Aktie* ht, Tindex* helpH) {
	printf("\nDer Kurs welcher Aktie soll ausgelesen werden? (Name/Kürzel:) ");

	Aktie* ap = checkInput(ht, helpH);
	if (!ap) return;

	if (ap->history[0].date[0] == '\0') {
		printf("\nKeine Kursdaten vorhanden.\n");
		return;
	}

	printf("\nLetzter Kurs der Aktie %s\nDatum:  %s\nClose: $ %.2f\nVolume: %d\nOpen: $ %.2f\nHigh: $ %.2f\nLow: $ %.2f\n",
	       ap->name, ap->history[0].date, ap->history[0].close, ap->history[0].volume,
	       ap->history[0].open, ap->history[0].high, ap->history[0].low);
}

void plot(Aktie* ht, Tindex* helpH) {
	printf("\nWelcher Aktienkurs soll geplotet werden? (Name / Kürzel): ");

	Aktie* ap = checkInput(ht, helpH);
	if (!ap) return;

    int n = (ap->history[0].date[0] != '\0') ? 30 : 0;
    if (n == 0) {
        printf("\nKeine Kursdaten vorhanden.\n");
        return;
    }

	float min = ap->history[0].close;
	float max = ap->history[0].close;

	for (int i = 1; i < n; i++) {
		if (ap->history[i].close < min) min = ap->history[i].close;
		if (ap->history[i].close > max) max = ap->history[i].close;
	}

	int height = (int)(max - min);
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			if ((int)(ap->history[j].close - min) < height - i) printf(" ");
			else printf("$");
		}
		printf("\n");
	}
}

void save(Aktie* ht) {
	printf("\nName der Speicherdatei angeben: ");
	char input[256];
	fgets(input, sizeof(input), stdin);
	trimNewline(input);

	char filename[300];
	snprintf(filename, sizeof(filename), "%s.csv", input);

	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("\nERROR: File konnte nicht geöffnet werden");
		return;
	}

	for (int i = 0; i < HT_SIZE; i++) {
		if (!isEmptyAktie(&ht[i]) && !isDeletedAktie(&ht[i])) {
			fprintf(file, "name: %s, wkn: %s, krzl: %s\n", ht[i].name, ht[i].wkn, ht[i].krzl);
            for (int j = 0; j < 30; j++) {
                if (ht[i].history[j].date[0] == '\0') break;
                fprintf(file, "%s,$%f,%d,$%f,$%f,$%f\n",
                    ht[i].history[j].date, ht[i].history[j].close, ht[i].history[j].volume,
                    ht[i].history[j].open, ht[i].history[j].high, ht[i].history[j].low);
            }
			fprintf(file, "\n");
		}
	}

	fclose(file);
	printf("\nDaten wurden gespeichert.");
}

void deleteHelpHt(Tindex* helpH) {
	for (int i = 0; i < HELPH_SIZE; i++) {
		helpH[i] = (Tindex){0};
		helpH[i].idx = -1;
	}
}

void deleteHt(Aktie* ht) {
	for (int i = 0; i < HT_SIZE; i++) {
		ht[i] = (Aktie){0};
	}
}

void load(Aktie* ht, Tindex* helpH) {
    printf("\nName der Speicherdatei angeben (Dateipfad): ");
    char input[256];
    fgets(input, sizeof(input), stdin);
    trimNewline(input);

    FILE* file = fopen(input, "r");
    if (file == NULL) {
        printf("\nERROR: File konnte nicht geöffnet werden");
        return;
    }

    deleteHt(ht);
    deleteHelpHt(helpH);

    char line[512];
    while (fgets(line, sizeof(line), file)) { //Auslesen aus datei, line für line
        trimNewline(line);
        if (strlen(line) == 0) continue; // leere Zeilen überspringen

        Aktie temp = {0};
        if (sscanf(line, "name: %31[^,], wkn: %31[^,], krzl: %31[^,\n]", temp.name, temp.wkn, temp.krzl) != 3) {
            continue; // Fehlerhafte Zeile überspringen
        }

        // 30 Kursdaten einlesen
        for (int i = 0; i < 30; i++) {
            if (!fgets(line, sizeof(line), file)) break; //Ende Datei
            trimNewline(line);
            if (strlen(line) == 0) break; //Leere Zeile - nächst Aktie

            if (sscanf(line, "%10[^,],$%f,%d,$%f,$%f,$%f",
                   temp.history[i].date,
                   &temp.history[i].close,
                   &temp.history[i].volume,
                   &temp.history[i].open,
                   &temp.history[i].high,
                   &temp.history[i].low) != 6) {
            // fehlerhafte Zeile überspringen
            temp.history[i] = (Day){0}; }
        }

        // Aktie in Tabelle einfügen
        int i = 0;
        while (i < HT_SIZE) {
            int id = (hash(temp.name, 0) + i * i) % HT_SIZE;
            if (isEmptyAktie(&ht[id]) || isDeletedAktie(&ht[id])) {
                ht[id] = temp;
                updateHelpH(helpH, ht[id].name, id);
                updateHelpH(helpH, ht[id].krzl, id);
                break;
            }
            i++;
        }
    }

    fclose(file);
    printf("\nDaten wurden geladen.");
}
