#ifdef LASAL_VERSION
#if LASAL_VERSION > 57
//LASAL_INCLUDES
 #pragma once
 
#endif//LASALVERSION > 57
#endif//LASAL_VERSION
//**********************************************************************************  
//**                                                                              **
//**                     �nderungsvermerk Lasal TEXT Kern                         **
//**                                                                              **
//**********************************************************************************  


//***************************** Version Et_Kern_0.97g ******************************
// 29.03.2002
// Beim Editieren einer Variable wurde die Ober u. Untergrenze  gepr�ft
// bevor der Wert mit Enter best�tigt wurde.

// Variable Untergrenze 20, Obergrenze 100
// Problem: Beim dr�cken der ersten 2 wurde bereits <<< ausgegeben.



//***************************** Version Et_Kern_1.02 ******************************* 
// Sept. 2002
// Alle Typen wurden auf ET_... ge�ndert -> Grund gleiche Typenbezeichnung im LSE



//***************************** Version Et_Kern_1.04beta *************************** 
// Febr. 2003
// Nur Versionsname ge�ndert wegen gleicher Bezeichnung wie der Texteditor 



//****************************** Version Et_Kern_1.05 ******************************
// 5.5.2003
// Notwendige �nderung weil die CNC Klasse (Achsmakro) und der _Et_Runtime eine gleiche 
// Typenbezeichnung verwenden. Von "D_Set" auf "_ET_Set" ge�ndert. 



//****************************** Version Et_Kern_1.10 ******************************
// 5.7.2003
// �nderung im Headerfile ET_RunTimeTypes.h. Vor und nach dem Typ ET_COMDEF ein
// push und pop eingef�gt.

//     #pragma pack(push, 1)                // Zeile eingef�gt
//       ET_COMDEF : STRUCT            
//         typ :      ET_INTERFACE;
//         adress :      UINT;
//         handle :     ^VOID;
//         login :      ET_LOGIN;
//       END_STRUCT;
//     #pragma pack(pop)                    // Zeile eingef�gt



//****************************** Version Et_Kern_1.11 ******************************
// 26.9.2003
// Funktion "Get_ANYTHING" von private auf public ge�ndert, um auf die Textlisten
// zugreifen zu k�nnen.



//****************************** Version Et_Kern_1.12 ******************************
// 28.1.2004
// �nderung in der Funktion PLC_LogIn wegen neuem Loader 

//  if state = 0 then
//	  if OPS.uiLoaderVersion.usHiRev >= 2 then        // Zeile eingef�gt
//	    if OPS.uiLoaderVersion.usLoRev >= 2 then      // Zeile eingef�gt
//		  CallBack.ptr      := #irq_PLC2()$^USINT;    // Zeile eingef�gt
//		  CallBack.pComdata := cd^.handle$^Comdata;   // Zeile eingef�gt
//		  CallBack.pThis    := this$^UINT;            // Zeile eingef�gt
//		  INSTALLCALLBACK(#CallBack);                 // Zeile eingef�gt
//	    end_if;                                       // Zeile eingef�gt
//	  end_if;                                         // Zeile eingef�gt
//	  cd^.login := ET_LOGIN_TRUE;
//  end_if; 

// * Zus�tzlich neuer Typ in der _ET_RunTime Klasse 
//		CallBack		STRUCT
//    	   ptr			^USINT
//    	   pComdata		^COMDATA

// * Methode irq_PLC2 eingebaut, wegen Aufruf vom Loader

// * Methode GetCOMDEF erweitert f�r mehrere Canbusteilnehmer



//****************************** Version Et_Kern_1.13 ******************************
// 7.6.2004
// Zwei neue Funktionen in der Klasse _ET_Runtime f�r Vorzeichenwechsel einer Variable

// Control_VarNeg(); und Control_VarPos();

// Diese Methoden k�nnen von einer anderen Klasse �ber einen Objektkanal aufgerufen werden.
// Der Vorzeichenwechsel funktioneiert nur, wenn auf der Variable der Cursor steht.



//***************************** Version Et_Kern_1.14 ******************************
// 2.9.2004

// Die Methode FC_User der Klasse ET_User wurde von "FUNCTION GLOBAL" auf
// "FUNCTION VIRTUAL GLOBAL" ge�ndert, um diese ableiten zu k�nnen.

// In der Funktion "Out_IO_Intern" wurde f�r den Schritt IF_NUMERIC: zwei zeilen eingef�gt.
// Dies ist nur wenn eine Variable im Display platziert wird mit negativen Vorzeichen,
// wird der Stellenwert um eins erh�ht. 

// IF_NUMERIC:     width := (pv^.info.format and 16#000F);
//                 if(pv^.info.format and 16#00F0) then
//                   width += 1; 
//                 end_if;
//	               if(pv^.info.format and 16#0F00) then  //and 16#0F00
//  eingef�gt ->     if pv^.info.vartyp=ET_BYTE | pv^.info.vartyp=ET_WORD | pv^.info.vartyp=ET_DWORD then
//	                   width += 1;
//  eingef�gt ->     end_if;
//	              end_if;



//***************************** Version Et_Kern_1.15 ******************************
// 9.12.2004

// Programm�nderung in der Funktion PLC_LogIn und LoadProject
// Abfrage musste ge�ndert werden wegen neuer Versionsnummer vom Loader
// Ab Lasalversion 01.01.002 hat diese Abfrage nicht mehr funktioniert

// Abfrage vorher	  if OPS.uiLoaderVersion.usHiRev >= 2 then
//                      if OPS.uiLoaderVersion.usLoRev >= 2 then
 

//	  if OPS.uiLoaderVersion$UINT >= 16#0202 then	  // <--- ge�ndert
//      .  
//		.  
//		.  
//	  end_if;


//***************************** Version Et_Kern_1.16 ******************************
// 7.11.2005

// Programm�nderung in der Funktion SetSystemdata wegen Ober und Untergrenzen
// von Variablen beim Editieren. Werden jetzt die Ober und Untergrenzen beim 
// editieren einer Variable verletzt, erh�lt die Variable den Wert der Obergrenze
// bzw. Untergrenze.
// Vorher wurde der wert auf den Server geschrieben und die Zeichen <<< bzw. >>> ausgegeben. 

// Folgende Zeilen wurden eingef�gt.

//    if (pr^.value > pr^.hi_limit) then
//      pr^.value := pr^.hi_limit;  // Set variable to upper limit
//	  elsif (pr^.value < pr^.lo_limit) then
//      pr^.value := pr^.lo_limit;  // Set variable to lower limit
//	  end_if;


//***************************** Version Et_Kern_1.17 ******************************
// 18.5.2006

// Umbau des CyWork Task der _ET_RunTime Klasse auf Background Task.
// Grund: Aufruf der Funktion TXCOMMAND im CyWork Task der _ET_RunTime Klasse.   

// Weiterer Umbau: Folgender Programmcode im Background Task f�r das 
//                 blinken des Cursors beim Editieren von Variablen eingebaut.
//                 Zus�tzlich wurde ein neuer Client "Cursortime" f�r die 
//                 Blinkzeit der Cursors angelegt.                  
//

//    if(Cursor.active = TRUE) then
//      if (ops.tAbsolute - timecursor) >= TO_UDINT(Cursortime) then
//        if(Cursor.status = TRUE) then
//          CursorShow(FALSE);
//        else 
//          CursorShow(TRUE);
//        end_if; 
//      end_if;
//    else 
//      timecursor := ops.tAbsolute;
//    end_if;

//***************************** Version Et_Kern_1.18 ******************************
// 8.1.2008

// Von der _ET_Runtime Klasse wurden die Text Terminal Files entfernt.
//      Verlist.st
//      CharTable.st
//      KeyTable.st
//      PicList.st
//      Textlisten.st

// Da die _ET_Runtime Klass nun zu den Sigmatek Klassen geh�rt, kann diese durch 
// den Anwender nicht mehr ge�ndert werden (Austausch der Text Terminal Files).

// Die Text Terminal Files m�ssen jetzt unter 
//          File, Source Files, mit Add Existing File... eingebunden werden. 

//***************************** -------------------- ******************************


