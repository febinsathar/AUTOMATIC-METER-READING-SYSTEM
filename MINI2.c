#include <18F4550.h>
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN  
#use delay(clock=48000000) 
#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7,parity=N,bits=8,stream=gsm) 

#priority EXT2,RDA

#include <JHDGLCDNEW.c>
#include <math.h>
#include <ds1307.c> 
#include <stdio.h>
#include <stdlib.h>
#include <usb_cdc.h>
#define RANGE_BAR_START 10
#define PULSERATE 10
#define BILLeeptr 100
#define FAST_GLCD
char CusName[11]={"Adam"};
char ComPhno[11]={"9895491060"};
char CusPhno[11]={"8129385909"};
char CusID[3]={"29"};
char PeakTimeStartHr[3]={"19"};
char PeakTimeStopHr[3]={"22"};
int Billp;
char BillPulse[4]={"000"};
int BillAmount=0;
int count=0;
char eeptr,z,i,j,lowrly,a,b;

volatile int  counter_read=0;             //counter to fill the array
char *ptr;
char del[]={"at+cmgd=1\r"};
char snd[]={"at+cmgs=\"+919400355255\"\r"};
char sms[]={"hi"};
char bill[]={"bill"};
char offrly[]={"off"};
char onrly[]={"on"};
unsigned char rec[]={"at+cmgr=1\r"};
unsigned char StrCMTI[]={"+CMTI"};
unsigned char StrCSQ[]={"+CSQ:"};
volatile Char Recieve_String[200];   //buffer for incoming data
char phno[20];
char body[20];

#int_ext2
void isr(void)
{
   count++;
   if(count==PULSERATE)
   {
      count=0;
      Billp++;
   }
}

#INT_RDA
void SerialInt()
{
   Recieve_String[counter_read]=getchar();
   counter_read++;
   if(counter_read==199)counter_read=0;
}
void gsminit()
{
   printf("at");
   putchar(0x0D);
   delay_us(5);
   fprintf(gsm,"ate0\r");
   delay_us(5);
   fprintf(gsm,"at+cmgf=1\r");
   delay_us(5);
}
void clrbuffer()
{
 for(z=0;z<=99;z++)
   Recieve_String[z]='\0';
   counter_read=0;
}
void sendsms(char* sms,char* ph)
{
   for(z=12;z<=21;z++)
   snd[z]=ph[z-12];
   counter_read=0;
   fprintf(gsm,"%s",snd);
   delay_us(5);
   fprintf(gsm,"%s",sms);
   delay_us(5);
   fprintf(gsm,"%c",0x1A);
   delay_ms(3000);
   clrbuffer();
}
void delallsms()
{
   for(z=48;z<=57;z++)
   { 
      del[8]=z;
      fprintf(gsm,"%s",del);
      clrbuffer();   
   }
}

void extract()
{
i=0;
j=0;
while(Recieve_String[i]!=',')
      i++;
      i++;
      
      while(Recieve_String[i]!=',')
       {
         if(j>2)
         phno[j-3]=Recieve_String[i+1];
         
         i++;
         j++;
       }
       j-=2;
       phno[j-3]='\0';
       j=0;
      while(Recieve_String[i]!='\r')
         i++;
      i+=2;
     
      while(Recieve_String[i]!='\r')
      {
         body[j]=Recieve_String[i];
         i++;
         j++;
      }
     
      body[j]='\0';
      clrbuffer();
      
   }


void write()
   {  
      eeptr=0;
      for(i=0;i<=11;i++)
      {
         write_eeprom(eeptr,CusName[i] );
         eeptr++;
      }
      eeptr--;
      for(i=0;i<=11;i++)
      {
         write_eeprom(eeptr,ComPhno[i] );
         eeptr++;
      }
      eeptr--;
      for(i=0;i<=11;i++)
      {
         write_eeprom(eeptr,CusPhno[i] );
         eeptr++;
      }
      eeptr--;
      for(i=0;i<=3;i++)
      {
         write_eeprom(eeptr,CusID[i] );
         eeptr++;
      }
      eeptr--;
      for (i=0;i<=3;i++)
      {
         write_eeprom(eeptr,PeakTimeStartHr[i]);
         eeptr++;
      }
      eeptr--;
      for(i=0;i<=3;i++)
      {
         write_eeprom(eeptr,PeakTimeStopHr[i]);
         eeptr++;
      }
      eeptr--;
   }

void read()
   {  
      eeptr=0;
      for(i=0;i<=11;i++)
      {
         CusName[i]=read_eeprom(eeptr);
         eeptr++;
      }
      eeptr--;
      for(i=0;i<=11;i++)
      {
         ComPhno[i]=read_eeprom(eeptr);
         eeptr++;
      }
      eeptr--;
      for(i=0;i<=11;i++)
      {
         CusPhno[i]=read_eeprom(eeptr);
         eeptr++;
      }
      eeptr--;
      for(i=0;i<=3;i++)
      {
         CusID[i]=read_eeprom(eeptr);
         eeptr++;
      }
      eeptr--;
      for (i=0;i<=3;i++)
      {
         PeakTimeStartHr[i]=read_eeprom(eeptr);
         eeptr++;
      }
      eeptr--;
      for(i=0;i<=3;i++)
      {
         PeakTimeStopHr[i]=read_eeprom(eeptr);
         eeptr++;
      }
      eeptr--;
      Billp=0;
      for(i=0;i<=4;i++)
      {
         BillPulse[i]=read_eeprom(BILLeeptr+i);
      }
      Billp=atoi(BillPulse);
         
   }





void main()
{
   byte day,month,yr,dow,hr,min,sec;
   char Text[]={"AMR"};
   char choice[]={"PRESS THE BUTTON FOR UPDATION MODE"};
   char user[]={"USER: "};
   char userid[]={"USER ID: "};
   char update[]={"UPDATING....."};
   char usbwan[]={"PLUG TO PC...."};
   char ar[]={"ok"};
   char i,c,temp,counter;
   char countdis[5];
   byte time_dis[30];
   
   unsigned char *ptr1;
   int8 prev;
   write();
   read();
   
      
   ds1307_init();
   //ds1307_set_date_time(21,3,12,4,11,32,0); 
   
   glcd_init(ON);
   glcd_img();
   glcd_text57(80,47,Text,2,ON);
   delay_ms(1000);
   glcd_fillScreen(OFF);
   glcd_text57(0,40,choice,1,ON);
   //delay_ms(2000);
   glcd_fillScreen(OFF);
   
     if(!input(PIN_A0))
         {  disable_interrupts(INT_EXT2);
            usb_cdc_init();
            usb_init();
            glcd_text57(10,10,usbwan,1,ON);
            
            usb_wait_for_enumeration();
         
            
           while(true) 
            {
      
               usb_task();
               if(usb_cdc_kbhit())
               { 
                  glcd_fillScreen(OFF);
                  glcd_text57(10,10,update,1,ON);  
               
                  printf(usb_cdc_putc, "\n\n\rAMR\n\r"); 
                  printf(usb_cdc_putc, "\r1.CUSTOMER NAME: %s\r\n",CusName);
                  printf(usb_cdc_putc, "\r2.COMPANY PHONE NO: %s\r\n",ComPhno);
                  printf(usb_cdc_putc, "\r3.CUSTOMER PHONE NO: %s\r\n",CusPhno);
                  printf(usb_cdc_putc, "\r4.CUSTOMER ID: %s\r\n",CusID);
                  printf(usb_cdc_putc, "\r5.PeakTimeStartHr: %s:00\r\n",PeakTimeStartHr);
                  printf(usb_cdc_putc, "\r6.PeakTimeStopHr: %s:00\r\n",PeakTimeStopHr);
                  printf(usb_cdc_putc, "\rBILL PULSE: %s\r\n",BillPulse);
                  printf(usb_cdc_putc, "\rBILL AMOUNT:RS %s/-\r\n",BillPulse);
                  printf(usb_cdc_putc, "\r7.SAVE\r\n");
                  printf(usb_cdc_putc, "\rENTER CHOICE TO UPDATE:\r\n");
         
                  c = usb_cdc_getc();
           
                  SWITCH(c)
                  {
                     case '1':counter=0;
                              printf(usb_cdc_putc, "\r1.ENTER NEW CUSTOMER NAME: \r\n");
                              temp=usb_cdc_getc();
                              while(temp!='*')
                              {
                                 printf(usb_cdc_putc, "%c",temp);
                                 CusName[counter]=temp;
                                 temp=usb_cdc_getc();
                                 counter++;
                              }
                              CusName[counter]='\0';
                              write();
                              break;
               
                     case '2':counter=0;
                              printf(usb_cdc_putc, "\r2.ENTER NEW COMPANY PHONE NO: \r\n");
                              temp=usb_cdc_getc();
                              while(temp!='*')
                              {
                              printf(usb_cdc_putc, "%c",temp);
                              ComPhno[counter]=temp;
                              temp=usb_cdc_getc();
                              counter++;
                              }
                              ComPhno[counter]='\0';
                              write();
                              break;
               
                     case '3':counter=0;
                              printf(usb_cdc_putc, "\r1.ENTER NEW CUSTOMER PHONE NO: \r\n");
                              temp=usb_cdc_getc();
                              while(temp!='*')
                              {
                                 printf(usb_cdc_putc, "%c",temp);
                                 CusPhno[counter]=temp;
                                 temp=usb_cdc_getc();
                                 counter++;
                              }
                              CusPhno[counter]='\0';
                              write();
                              break;
               
                     case '4':counter=0;
                              printf(usb_cdc_putc, "\r1.ENTER NEW CUSTOMER ID: \r\n");
                              temp=usb_cdc_getc();
                              while(temp!='*')
                              {
                                 printf(usb_cdc_putc, "%c",temp);
                                 CusID[counter]=temp;
                                 temp=usb_cdc_getc();
                                 counter++;
                              }
                              CusID[counter]='\0';
                              write();
                              break;
                        
                     case '5':counter=0;
                              printf(usb_cdc_putc, "\r1.ENTER NEW Peak Time Start Hour: \r\n");
                              temp=usb_cdc_getc();
                              while(temp!='*')
                              {
                                 printf(usb_cdc_putc, "%c",temp);
                                 PeakTimeStartHr[counter]=temp;
                                 temp=usb_cdc_getc();
                                 counter++;
                              }
                              PeakTimeStartHr[counter]='\0';
                              write();
                              break;
               
                     case '6':counter=0;
                              printf(usb_cdc_putc, "\r1.ENTER Peak Time Stop Hour: \r\n");
                              temp=usb_cdc_getc();
                              while(temp!='*')
                              {
                                 printf(usb_cdc_putc, "%c",temp);
                                 PeakTimeStopHr[counter]=temp;
                                 temp=usb_cdc_getc();
                                 counter++;
                              }
                              PeakTimeStopHr[counter]='\0';
                              write();
                              break;
                     case '7': write();
                              goto x;
                              break;
                     }
               
                
                  }
               }
         } 
     else 
      { 
         
       x:
         glcd_fillScreen(OFF);
         read();
         enable_interrupts(GLOBAL);
         enable_interrupts(INT_RDA);
         enable_interrupts(INT_EXT2);
         ext_int_edge(2,L_TO_H);
         glcd_rect(6,0,7,11,YES,ON);
         glcd_line(2,0,11,0,ON);
         glcd_line(2,1,6,5,ON);
         glcd_line(2,2,6,6,ON);
         glcd_line(11,1,6,6,ON);
         glcd_line(11,2,6,7,ON);
         glcd_text57(40,0,Text,2,ON);
         glcd_rect(38, 30, 127, 63, YES, OFF);
         glcd_text57(45,30,USER,1,ON);
         glcd_text57(75,30,CusName,1,ON);
         glcd_text57(18,30,BillPulse,1,ON);
         gsminit();
         delallsms();
         clrbuffer();
         lowrly=1;
         //sendsms(sms,ComPhno);
         prev=Billp;
         sprintf(body,"on");
         glcd_rect(90, 45, 127, 63, YES, OFF);
         glcd_text57(100,45,body,1,ON);

         while(1)
         {
            if(Billp>prev)
            {
            sprintf(BillPulse,"%d",Billp);
            for(i=0;i<=4;i++)
            write_eeprom(BILLeeptr+i,BillPulse[i]);
            prev=Billp;
            glcd_rect(18, 30, 30, 63, YES, OFF);
            glcd_text57(18,30,BillPulse,1,ON);
            }
            ds1307_get_date( day, month, yr, dow);
            ds1307_get_time( hr, min, sec );
            sprintf(time_dis,"%2u/%2u/20%2u %2u:%2u:%2u",day,month,yr,hr,min,sec);
            glcd_rect(0, 19, 127, 27, YES, ON);
            glcd_text57(1,20,time_dis,1,OFF);

            sprintf(countdis,"%d",count);
            glcd_rect(5, 30, 15, 44, YES, OFF);
            glcd_text57(5,30,countdis,2,ON);
            sprintf(BillPulse,"%d",Billp); 
            
            fprintf(gsm,"at+csq\r");
            delay_ms(5);
            i=0;
            while(Recieve_String[i]!=':'){i++;}
            i+=2;
            z=Recieve_String[i];
            if(z=='3')
            z=12;
            if(z=='2')
            z=9;
            if(z=='1')
            z=6;
            else
            z=3;
            z=12;
            for(i=RANGE_BAR_START,j=8;i<=(RANGE_BAR_START+z);i+=3,j-=2)
            glcd_rect(i,j, i+1, 11, NO, ON);

            counter_read=0;
            ptr=strstr (Recieve_String,StrCMTI);
            if(ptr!=NULL)
            {
               rec[8]=*(ptr+12);
               counter_read=0;
               fprintf(gsm,"%s",rec);
      
               delay_ms(200);
               extract();
               glcd_rect(0, 45, 90, 63, YES, OFF);
               glcd_text57(5,45,phno,1,ON);
               glcd_text57(5,55,body,1,ON);

               a=strcmp(phno,ComPhno);
               b=strcmp(phno,CusPhno);
               if((a==0)||(b==0))
               {  
                  //if(strcmp(body,bill)==0)
                  if((body[0]=='b')&&(body[1]=='i')&&(body[2]=='l')&&(body[3]=='l'))
                     {
                        sprintf(sms,"pulse:%d",billp);
                        sendsms(sms,phno);
                     }
                  
                  //if ((strcmp(body,offrly)==0))
                  if((body[0]=='o')&&(body[1]=='f')&&(body[2]=='f'))
                  {
                  if(a==0)
                  lowrly=0;
                  
                  output_high(pin_E1);
                  glcd_rect(90, 45, 127, 63, YES, OFF);
                  glcd_text57(100,45,body,1,ON);
                  sprintf(sms,"TURND OFF%d",billp);
                  sendsms(sms,phno);
                  }
               
                  //if ((strcmp(body,onrly)==0))
                  if((body[0]=='o')&&(body[1]=='n'))
                  {
                     if(a==0)
                     lowrly=1;
                     
                     
                     if(lowrly==1)
                     {  
                        glcd_rect(90, 45, 127, 63, YES, OFF);
                        glcd_text57(100,45,body,1,ON);
                        output_low(pin_E1);
                        sprintf(sms,"TURND ON%d",billp);
                        sendsms(sms,phno);
                     }
                     else
                     {
                        sprintf(sms,"NO ACCESS");
                        sendsms(sms,phno);
                     }
                  }
               }
               else
               {
                        sprintf(sms,"NO ACCESS");
                        sendsms(sms,phno);
               }
               
                 
       } 
 del[8]=rec[8];
fprintf(gsm,"%s",del);



              //clrbuffer();
            
            
            
            //

            
            delay_ms(1000);
         }
      
      //}
   

}


