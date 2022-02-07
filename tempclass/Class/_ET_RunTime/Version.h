#ifdef LASAL_VERSION
#if LASAL_VERSION > 57
//LASAL_INCLUDES
 #pragma once
 
#endif//LASALVERSION > 57
#endif//LASAL_VERSION
//**********************************************************************************  
//**                                                                              **
//**                     Änderungsvermerk Lasal TEXT Kern                         **
//**                                                                              **
//**********************************************************************************  


//***************************** Version Et_Kern_0.97g ******************************
// 29.03.2002
// Beim Editieren einer Variable wurde die Ober u. Untergrenze  geprüft
// bevor der Wert mit Enter bestätigt wurde.

// Variable Untergrenze 20, Obergrenze 100
// Problem: Beim drücken der ersten 2 wurde bereits <<< ausgegeben.



//***************************** Version Et_Kern_1.02 ******************************* 
// Sept. 2002
// Alle Typen wurden auf ET_... geändert -> Grund gleiche Typenbezeichnung im LSE



//***************************** Version Et_Kern_1.04beta *************************** 
// Febr. 2003
// Nur Versionsname geändert wegen gleicher Bezeichnung wie der Texteditor 



//****************************** Version Et_Kern_1.05 ******************************
// 5.5.2003
// Notwendige Änderung weil die CNC Klasse (Achsmakro) und der _Et_Runtime eine gleiche 
// Typenbezeichnung verwenden. Von "D_Set" auf "_ET_Set" geändert. 



//****************************** Version Et_Kern_1.10 ******************************
// 5.7.2003
// Änderung im Headerfile ET_RunTimeTypes.h. Vor und nach dem Typ ET_COMDEF ein
// push und pop eingefügt.

//     #pragma pack(push, 1)                // Zeile eingefügt
//       ET_COMDEF : STRUCT            
//         typ :      ET_INTERFACE;
//         adress :      UINT;
//         handle :     ^VOID;
//         login :      ET_LOGIN;
//       END_STRUCT;
//     #pragma pack(pop)                    // Zeile eingefügt



//****************************** Version Et_Kern_1.11 ******************************
// 26.9.2003
// Funktion "Get_ANYTHING" von private auf public geändert, um auf die Textlisten
// zugreifen zu können.



//****************************** Version Et_Kern_1.12 ******************************
// 28.1.2004
// Änderung in der Funktion PLC_LogIn wegen neuem Loader 

//  if state = 0 then
//	  if OPS.uiLoaderVersion.usHiRev >= 2 then        // Zeile eingefügt
//	    if OPS.uiLoaderVersion.usLoRev >= 2 then      // Zeile eingefügt
//		  CallBack.ptr      := #irq_PLC2()$^USINT;    // Zeile eingefügt
//		  CallBack.pComdata := cd^.handle$^Comdata;   // Zeile eingefügt
//		  CallBack.pThis    := this$^UINT;            // Zeile eingefügt
//		  INSTALLCALLBACK(#CallBack);                 // Zeile eingefügt
//	    end_if;                                       // Zeile eingefügt
//	  end_if;                                         // Zeile eingefügt
//	  cd^.login := ET_LOGIN_TRUE;
//  end_if; 

// * Zusätzlich neuer Typ in der _ET_RunTime Klasse 
//		CallBack		STRUCT
//    	   ptr			^USINT
//    	   pComdata		^COMDATA

// * Methode irq_PLC2 eingebaut, wegen Aufruf vom Loader

// * Methode GetCOMDEF erweitert für mehrere Canbusteilnehmer



//****************************** Version Et_Kern_1.13 ******************************
// 7.6.2004
// Zwei neue Funktionen in der Klasse _ET_Runtime für Vorzeichenwechsel einer Variable

// Control_VarNeg(); und Control_VarPos();

// Diese Methoden können von einer anderen Klasse über einen Objektkanal aufgerufen werden.
// Der Vorzeichenwechsel funktioneiert nur, wenn auf der Variable der Cursor steht.



//***************************** Version Et_Kern_1.14 ******************************
// 2.9.2004

// Die Methode FC_User der Klasse ET_User wurde von "FUNCTION GLOBAL" auf
// "FUNCTION VIRTUAL GLOBAL" geändert, um diese ableiten zu können.

// In der Funktion "Out_IO_Intern" wurde für den Schritt IF_NUMERIC: zwei zeilen eingefügt.
// Dies ist nur wenn eine Variable im Display platziert wird mit negativen Vorzeichen,
// wird der Stellenwert um eins erhöht. 

// IF_NUMERIC:     width := (pv^.info.format and 16#000F);
//                 if(pv^.info.format and 16#00F0) then
//                   width += 1; 
//                 end_if;
//	               if(pv^.info.format and 16#0F00) then  //and 16#0F00
//  eingefügt ->     if pv^.info.vartyp=ET_BYTE | pv^.info.vartyp=ET_WORD | pv^.info.vartyp=ET_DWORD then
//	                   width += 1;
//  eingefügt ->     end_if;
//	              end_if;



//***************************** Version Et_Kern_1.15 ******************************
// 9.12.2004

// Programmänderung in der Funktion PLC_LogIn und LoadProject
// Abfrage musste geändert werden wegen neuer Versionsnummer vom Loader
// Ab Lasalversion 01.01.002 hat diese Abfrage nicht mehr funktioniert

// Abfrage vorher	  if OPS.uiLoaderVersion.usHiRev >= 2 then
//                      if OPS.uiLoaderVersion.usLoRev >= 2 then
 

//	  if OPS.uiLoaderVersion$UINT >= 16#0202 then	  // <--- geändert
//      .  
//		.  
//		.  
//	  end_if;


//***************************** Version Et_Kern_1.16 ******************************
// 7.11.2005

// Programmänderung in der Funktion SetSystemdata wegen Ober und Untergrenzen
// von Variablen beim Editieren. Werden jetzt die Ober und Untergrenzen beim 
// editieren einer Variable verletzt, erhält die Variable den Wert der Obergrenze
// bzw. Untergrenze.
// Vorher wurde der wert auf den Server geschrieben und die Zeichen <<< bzw. >>> ausgegeben. 

// Folgende Zeilen wurden eingefügt.

//    if (pr^.value > pr^.hi_limit) then
//      pr^.value := pr^.hi_limit;  // Set variable to upper limit
//	  elsif (pr^.value < pr^.lo_limit) then
//      pr^.value := pr^.lo_limit;  // Set variable to lower limit
//	  end_if;


//***************************** Version Et_Kern_1.17 ******************************
// 18.5.2006

// Umbau des CyWork Task der _ET_RunTime Klasse auf Background Task.
// Grund: Aufruf der Funktion TXCOMMAND im CyWork Task der _ET_RunTime Klasse.   

// Weiterer Umbau: Folgender Programmcode im Background Task für das 
//                 blinken des Cursors beim Editieren von Variablen eingebaut.
//                 Zusätzlich wurde ein neuer Client "Cursortime" für die 
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

// Da die _ET_Runtime Klass nun zu den Sigmatek Klassen gehört, kann diese durch 
// den Anwender nicht mehr geändert werden (Austausch der Text Terminal Files).

// Die Text Terminal Files müssen jetzt unter 
//          File, Source Files, mit Add Existing File... eingebunden werden. 

//***************************** -------------------- ******************************


