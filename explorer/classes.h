#define MAX_EXPLORERS 100
#define MAX_CAVERNS 100
#define MAX_CAVERN_DESCRIPTION_LENGTH 200

class cavern {
  public:
   char description[MAX_CAVERN_DESCRIPTION_LENGTH];
   char directions_out[7];
   int leading_to[6];
   int number;
   int stuff;
   int explorers;
   cavern();
   cavern(char *d, char *p, int l[], int n, int s);
};

class explorer {
  public:
   int number;
   int stuff;
   int score;
   int current_cavern;
   explorer();
   explorer(int n, int c);
   ~explorer();
};

class cave {
  public:
  char items[16][25]={"keyboard",
                      "monitor",
                      "mouse",
                      "pencil",
                      "rod",
                      "tape",
                      "lamp",
                      "cursor",
                      "password",
                      "printer",
                      "database",
                      "chip",
                      "diskette",
                      "calculator",
                      "program",
                      "terminator"};

   cavern *array_of_caverns[MAX_CAVERNS];
   int number_of_caverns;
   explorer *array_of_explorers[MAX_EXPLORERS];
   int number_of_explorers;
   cave();
   void add(cavern *c);
   void add(explorer *e);
   int value(explorer *e);
   void remove(explorer *e);
   explorer* get_explorer(int n);
   cavern* get_cavern(int i);
   void look(explorer *e);
   void put(char *ch, explorer *e);
   void get(char *ch, explorer *e);
   void talk(char *ch, explorer *e);
   void quit(explorer *e);
   void move(char d, explorer *e);
};