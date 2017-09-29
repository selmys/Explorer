class cavern {
  public:
   char description[200];
   char directions_out[7];
   int leading_to[6];
   int number;
   int stuff;
   int explorers;
   cavern(){description[0]='\0'; directions_out[0]='\0'; 
            explorers=0; stuff=0;}
   cavern(char *d, char *p, int l[], int n, int s){
      strcpy(description,d);
      strcpy(directions_out,p);
      number=n;
      for(int i=0;i<6;i++)leading_to[i]=l[i];
      stuff=s;
      explorers=0;
   }
};

class explorer {
  public:
   int number;
   int stuff;
   int score;
   int current_cavern;
   explorer(){number=0; score=0; stuff=0; current_cavern=0;}
   explorer(int n, int c){number=n; current_cavern=c; score=0; stuff=0;}
   ~explorer(){printf("deleting explorer number %d\n",number);}
};

class cave {
  public:
   cavern *array_of_caverns[100];
   int number_of_caverns;
   explorer *array_of_explorers[100];
   int number_of_explorers;
   cave(){ number_of_caverns=number_of_explorers=0;}
   void add(cavern *c){ 
       array_of_caverns[number_of_caverns++]=c;}
   void add(explorer *e){
       array_of_explorers[number_of_explorers++]=e;
       array_of_caverns[e->current_cavern]->explorers |= (1 << e->number);
   }
   int value(explorer *e){
       char ch[1024]="Your score is ";
       char cc[10]="";
       int i,t=0;
       for(i=0;i<32;i++)
          if(e->score & (1 << i))t+=1;
       for(i=0;i<32;i++)
          if(e->stuff& (1 << i))t+=1;
       t*=10;
       sprintf(cc,"%d",t);
       strcat(ch,cc);
       strcat(ch,"\n");
       write(e->number,ch,strlen(ch));
       return t;
   }
   void remove(explorer *e){
       int i,j,pos;
       for(i=0;i<number_of_explorers;i++)
          if(array_of_explorers[i]==e)pos=i;
       for(i=pos;i<number_of_explorers-1;i++)
          array_of_explorers[i]=array_of_explorers[i+1];
       number_of_explorers-=1;
       array_of_caverns[e->current_cavern]->explorers ^= (1 << e->number);
       array_of_caverns[e->current_cavern]->stuff |= e->stuff;
       delete e;
   }
   explorer* get_explorer(int n){
       for(int i=0;i<number_of_explorers;i++)
          if(array_of_explorers[i]->number == n) return array_of_explorers[i];
       return 0;
   }
   cavern* get_cavern(int i){
       return array_of_caverns[i];
   }
   void look(explorer *e){
       char ch[1024]="",*ptr,hold[100]="";
       int i,f=0;
       strcat(ch,array_of_caverns[e->current_cavern]->description);
       strcat(ch,"\n");
       for(i=0;i<16;i++)
          if(array_of_caverns[e->current_cavern]->stuff & (1 << i)) {
             strcat(ch,"There is a ");
             strcat(ch,items[i]);
             strcat(ch," here.\n");
          }
       for(i=0;i<16;i++)
          if(e->stuff & (1 << i)){
               strcat(ch,"You are carrying a ");
               strcat(ch,items[i]);
               strcat(ch,".\n");
          }
       if(strlen(array_of_caverns[e->current_cavern]->directions_out)>1)
          strcat(ch,"There are passages leading");
       else
          strcat(ch,"There is a passage leading");
       for(i=0;array_of_caverns[e->current_cavern]->directions_out[i];i++){
         if(array_of_caverns[e->current_cavern]->directions_out[i]=='n'){
            if(f==1)strcat(ch,", north");
            else strcat(ch," north");
            f=1;
         } 
         if(array_of_caverns[e->current_cavern]->directions_out[i]=='s'){
            if(f==1)strcat(ch,", south");
            else strcat(ch, " south");
            f=1;
         } 
         if(array_of_caverns[e->current_cavern]->directions_out[i]=='e'){
            if(f==1)strcat(ch,", east");
            else strcat(ch, " east");
            f=1;
         }
         if(array_of_caverns[e->current_cavern]->directions_out[i]=='w'){
            if(f==1)strcat(ch,", west");
            else strcat(ch, " west");
            f=1;
         }
         if(array_of_caverns[e->current_cavern]->directions_out[i]=='u'){
            if(f==1)strcat(ch,", up");
            else strcat(ch, " up");
            f=1;
         }
         if(array_of_caverns[e->current_cavern]->directions_out[i]=='d')
            if(f==1)strcat(ch,", down");
            else strcat(ch, " down");
            f=1;
       }
       strcat(ch,".\n");
       ptr=rindex(ch,',');
       if(ptr){
         *ptr=0;
         strcpy(hold,ptr+1);
         strcat(ch," and");
         strcat(ch,hold);
       }
       if(array_of_caverns[e->current_cavern]->explorers ^ 
          (array_of_caverns[e->current_cavern]->explorers & 
             (1 << e->number)))
                strcat(ch,"There are other explorers here.\n");
       write(e->number,ch,strlen(ch));
   }
   void put(char *ch, explorer *e){
      int i=1000,j=0;
      char c[1024]="",cc[1024]="";
      for(j=2;j<strlen(ch);j++)
         c[j-2]=ch[j];
      c[j-2]='\0';
      for(j=0;j<16;j++)
         if(!strcmp(items[j],c))i=j;
      if(i<1000)
         if(e->stuff & (1 << i)){
            e->stuff ^= (1 << i);
            array_of_caverns[e->current_cavern]->stuff |= (1 << i);
            write(e->number,"ok.\n",4); }
         else {
           strcpy(cc,"You are not carrying any ");
           strcat(cc,c);
           strcat(cc," .\n");
           write(e->number,cc,strlen(cc));
         }
      else {
        strcpy(cc,"You are not carrying any ");
        strcat(cc,c);
        strcat(cc," .\n");
        write(e->number,cc,strlen(cc));
      }
   }
   void get(char *ch, explorer *e){
      int i=1000,j=0;
      char c[1024]="",cc[1024]="";
      for(j=2;j<strlen(ch);j++)
         c[j-2]=ch[j];
      c[j-2]='\0';
      for(j=0;j<16;j++)
         if(!strcmp(items[j],c))i=j;
      if(i<1000)
         if(array_of_caverns[e->current_cavern]->stuff & (1 << i)) {
            array_of_caverns[e->current_cavern]->stuff ^= (1 << i);
            e->stuff |= (1 << i);
            write(e->number,"ok.\n",4);}
         else {
           if(e->current_cavern == 4 && !strcmp(c,"table"))
              write(e->number,"The table is too heavy.\n",24);
           else {
              strcpy(cc,"I see no ");
              strcat(cc,c);
              strcat(cc," here.\n");
              write(e->number,cc,strlen(cc));
           }
         }
      else {
        if(e->current_cavern == 4 && !strcmp(c,"table"))
           write(e->number,"The table is too heavy.\n",24);
        else {
           strcpy(cc,"I see no ");
           strcat(cc,c);
           strcat(cc," here.\n");
           write(e->number,cc,strlen(cc));
        }
      }
   } 
   void talk(char *ch, explorer *e){
      int j;
      char c[1024]="";
      printf("$%s$%d\n",ch,strlen(ch));
      for(j=2;j<strlen(ch);j++)
         c[j-2]=ch[j];
      c[j]='\0';
      strcat(c,"\n");
      printf("$%s$%d\n",c,strlen(c));
      for(int i=0;i<32;i++)
         if(array_of_caverns[e->current_cavern]->explorers & (1 << i))
            if(i != e->number) write(i,c,strlen(c));
   } 
   void quit(explorer *e){
      value(e);
      write(e->number,"Q\n",2);
   }
   void move(char d, explorer *e){
      int s,pos=0,i;
      if(index(array_of_caverns[e->current_cavern]->directions_out, d)){
         for(i=0;i<7;i++)
           if(array_of_caverns[e->current_cavern]->directions_out[i]==d)
             pos=i;
         i=array_of_caverns[e->current_cavern]->leading_to[pos];
         array_of_caverns[e->current_cavern]->explorers ^= (1 << e->number);
         printf("Explorer %d moving to cavern %d\n",e->number,i);
         e->current_cavern=i;
         e->score |= (1 << i);
         array_of_caverns[e->current_cavern]->explorers |= (1 << e->number);
         for(i=0;i<32;i++)
            if(e->score & (1 << i))s+=1;
         for(i=0;i<32;i++)
            if(e->stuff& (1 << i))s+=1;
         s*=10;
         if((s>=200) && (e->current_cavern==0))
            quit(e);
      }
      else
         write(e->number,"There is no way to go in that direction.\n",41);
   }
};
