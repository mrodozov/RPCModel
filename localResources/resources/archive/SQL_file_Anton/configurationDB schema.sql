--------------------------------------------------------
--  File created - Thursday-April-14-2011   
--------------------------------------------------------

--------------------------------------------------------
--  DDL for Table DYNAMICPARAMVALUE
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."DYNAMICPARAMVALUE" 
   (	"DYNAMICPARAMVALUEID" NUMBER(11,0), 
	"DC_DYNAMICCONFID" NUMBER(11,0), 
	"DPD_DYNAMICPARAMDEFID" NUMBER(11,0), 
	"INTVALUE" NUMBER(11,0), 
	"FLOATVALUE" NUMBER, 
	"STRINGVALUE" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CRATE
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CRATE" 
   (	"CRATEID" NUMBER(11,0), 
	"TYPE" VARCHAR2(30), 
	"LABEL" VARCHAR2(30), 
	"NAME" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CONTROLBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CONTROLBOARD" 
   (	"CONTROLBOARDID" NUMBER(11,0), 
	"CCSBOARD_CCSBOARDID" NUMBER(11,0), 
	"POSITIONINCCUCHAIN" NUMBER(4,0), 
	"CCUADDRESS" NUMBER(4,0), 
	"FECPOSITION" NUMBER(4,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table INTARRAY24
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."INTARRAY24" 
   (	"INTARRAY24ID" NUMBER(11,0), 
	"VALUE0" NUMBER(11,0), 
	"VALUE1" NUMBER(11,0), 
	"VALUE2" NUMBER(11,0), 
	"VALUE3" NUMBER(11,0), 
	"VALUE4" NUMBER(11,0), 
	"VALUE5" NUMBER(11,0), 
	"VALUE6" NUMBER(11,0), 
	"VALUE7" NUMBER(11,0), 
	"VALUE8" NUMBER(11,0), 
	"VALUE9" NUMBER(11,0), 
	"VALUE10" NUMBER(11,0), 
	"VALUE11" NUMBER(11,0), 
	"VALUE12" NUMBER(11,0), 
	"VALUE13" NUMBER(11,0), 
	"VALUE14" NUMBER(11,0), 
	"VALUE15" NUMBER(11,0), 
	"VALUE16" NUMBER(11,0), 
	"VALUE17" NUMBER(11,0), 
	"VALUE18" NUMBER(11,0), 
	"VALUE19" NUMBER(11,0), 
	"VALUE20" NUMBER(11,0), 
	"VALUE21" NUMBER(11,0), 
	"VALUE22" NUMBER(11,0), 
	"VALUE23" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table DCCCHIPCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."DCCCHIPCONF" 
   (	"DCCCHIPSTATICCONFID" NUMBER(11,0), 
	"BCN0DELAY" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table DYNAMICCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."DYNAMICCONF" 
   (	"DYNAMICCONFID" NUMBER(11,0), 
	"TARGET" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table FEBCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."FEBCONF" 
   (	"FEBCONFID" NUMBER(11,0), 
	"FL_FEBLOCATIONID" NUMBER(11,0), 
	"THRESHHOLD" NUMBER(11,3)
   ) ;
 
--------------------------------------------------------
--  DDL for Table FEBLOCATION
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."FEBLOCATION" 
   (	"FEBLOCATIONID" NUMBER(11,0), 
	"LB_LINKBOARDID" NUMBER(11,0), 
	"CL_CHAMBERLOCATIONID" NUMBER(11,0), 
	"CBC_I2CCBCHANNELID" NUMBER(11,0), 
	"FEBLOCALETAPARTITION" VARCHAR2(20), 
	"FEBCMSETAPARTITION" VARCHAR2(20), 
	"POSINLOCALETAPARTITION" NUMBER(4,0), 
	"POSINCMSETAPARTITION" NUMBER(4,0), 
	"I2CLOCALNUMBER" NUMBER(4,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CRATEDISABLED
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CRATEDISABLED" 
   (	"CRATEDISABLEDID" NUMBER(11,0), 
	"CRATE_CRATEID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TBGBSORTCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TBGBSORTCONF" 
   (	"TBGBSORTCONFID" NUMBER(11,0), 
	"RECMUXCLKINV" RAW(18), 
	"RECMUXREGADD" RAW(18), 
	"IA4_RECMUXDELAYID" NUMBER(11,0), 
	"IA4_RECDATADELAYID" NUMBER(11,0), 
	"RECMUXCLK90" RAW(18) DEFAULT HEXTORAW(0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table RBCCHIPCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."RBCCHIPCONF" 
   (	"RBCCHIPSTATICCONFID" NUMBER(11,0), 
	"CONFIG" NUMBER(11,0), 
	"CONFIGIN" NUMBER(11,0), 
	"CONFIGVER" NUMBER(11,0), 
	"MAJORITY" NUMBER(11,0), 
	"SHAPE" NUMBER(11,0), 
	"MASK" NUMBER(11,0), 
	"CTRL" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table INTARRAY4
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."INTARRAY4" 
   (	"INTARRAY4ID" NUMBER(11,0), 
	"VALUE0" NUMBER(11,0), 
	"VALUE1" NUMBER(11,0), 
	"VALUE2" NUMBER(11,0), 
	"VALUE3" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table BACKPLANESORTERCONN
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."BACKPLANESORTERCONN" 
   (	"BACKPLANESORTERCONNID" NUMBER(11,0), 
	"TCBP_TCBACKPLANEID" NUMBER(11,0), 
	"SORTERBOARD_SORTERBOARDID" NUMBER(11,0), 
	"SORTERBOARDCONNECTOR" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table FEB
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."FEB" 
   (	"FEBID" NUMBER(11,0), 
	"FL_FEBLOCATIONID" NUMBER(11,0), 
	"DB_DISTRIBUTIONBOARDID" NUMBER(11,0), 
	"PRODUCTID" VARCHAR2(30), 
	"DATEOFINSTALLATION" DATE
   ) ;
 
--------------------------------------------------------
--  DDL for Table DYNAMICCONFDEF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."DYNAMICCONFDEF" 
   (	"DYNAMICCONFDEFID" NUMBER(11,0), 
	"VERSION" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CONFIGKEY_DEPR
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CONFIGKEY_DEPR" 
   (	"INVALID_KEY" VARCHAR2(500), 
	"INVALIDATION_DATE" TIMESTAMP (6) DEFAULT SYS_EXTRACT_UTC(SYSTIMESTAMP)
   ) ;
 
--------------------------------------------------------
--  DDL for Table DYNAMICPARAMDEF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."DYNAMICPARAMDEF" 
   (	"DYNAMICPARAMDEFID" NUMBER(11,0), 
	"DCD_DYNAMICCONFDEFID" NUMBER(11,0), 
	"VERSION" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table LINKDISABLED
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."LINKDISABLED" 
   (	"LINKDISABLEDID" NUMBER(11,0), 
	"BBC_BOARDBOARDCONNID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table FEBCONNECTOR
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."FEBCONNECTOR" 
   (	"FEBCONNECTORID" NUMBER(11,0), 
	"FL_FEBLOCATIONID" NUMBER(11,0), 
	"LINKBOARDINPUTNUM" NUMBER(4,0), 
	"FEBCONNECTORNUM" NUMBER(4,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table LOCALCONFIGKEY
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."LOCALCONFIGKEY" 
   (	"LOCALCONFIGKEYID" NUMBER(11,0), 
	"NAME" VARCHAR2(128), 
	"SUBSYSTEM" VARCHAR2(128)
   ) ;
 
--------------------------------------------------------
--  DDL for Table FIRMWARE
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."FIRMWARE" 
   (	"FIRMWAREID" NUMBER(11,0), 
	"DCD_DYNAMICCONFDEFID" NUMBER(11,0), 
	"REFERENCELOCATION" VARCHAR2(30), 
	"VERSION" VARCHAR2(30), 
	"DESCRIPTION" VARCHAR2(30), 
	"DATEOFCREATION" DATE, 
	"IIDREFERENCELOCATION" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CSCCHIPCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CSCCHIPCONF" 
   (	"CSCCHIPSTATICCONFID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TBRMBCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TBRMBCONF" 
   (	"TBRMBCONFID" NUMBER(11,0), 
	"CHANENA" NUMBER(11,0), 
	"PRETRIGGERVAL" NUMBER(11,0), 
	"POSTTRIGGERVAL" NUMBER(11,0), 
	"DATADELAY" NUMBER(11,0), 
	"TRGDELAY" NUMBER(11,0), 
	"RECMUXCLKINV" RAW(18), 
	"RECMUXREGADD" RAW(18), 
	"IA18_RECMUXDELAYID" NUMBER(11,0), 
	"IA18_RECDATADELAYID" NUMBER(11,0), 
	"RECMUXCLK90" RAW(18) DEFAULT HEXTORAW(0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CHAMBER
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CHAMBER" 
   (	"CHAMBERID" NUMBER(11,0), 
	"CL_CHAMBERLOCATIONID" NUMBER(11,0), 
	"PRODUCTID" VARCHAR2(30), 
	"DATEOFINSTALLATION" DATE, 
	"STRIPORDERSCHEMA" NUMBER(2,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table HOBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."HOBOARD" 
   (	"HOBOARDID" NUMBER(11,0), 
	"LOCATION" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CHAMBERTYPE
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CHAMBERTYPE" 
   (	"CHAMBERTYPEID" NUMBER(11,0), 
	"TYPENAME" VARCHAR2(30), 
	"SERVICEPOSITION" VARCHAR2(10), 
	"CHIMNEY" VARCHAR2(5), 
	"ETAPARTITIONCNT" NUMBER(4,0), 
	"FEBCNT" NUMBER(4,0), 
	"FEBCHAINCNT" NUMBER(4,0), 
	"FEBINCHAINCNT" NUMBER(4,0), 
	"STRIPCNT" NUMBER(4,0), 
	"STRIPINETAPARTITIONCNT" NUMBER(4,0), 
	"STRIPINFEBCNT" NUMBER(4,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table LINKTBCONN
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."LINKTBCONN" 
   (	"LINKTBCONNID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table FEBBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."FEBBOARD" 
   (	"FEBBOARDID" NUMBER(11,0), 
	"FL_FEBLOCATIONID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table BOARDDISABLED
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."BOARDDISABLED" 
   (	"BOARDDISABLEDID" NUMBER(11,0), 
	"BOARD_BOARDID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table XDAQAPPLBOXACCESS
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."XDAQAPPLBOXACCESS" 
   (	"XDAQAPPLBOXACCESSID" NUMBER(11,0), 
	"CCS_CCSBOARDID" NUMBER(11,0), 
	"FECPOSITION" NUMBER(4,0), 
	"PCISLOT" NUMBER(4,0) DEFAULT NULL, 
	"POSINVMECHAIN" NUMBER(4,0) DEFAULT NULL
   ) ;
 
--------------------------------------------------------
--  DDL for Table XDAQAPPLICATION
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."XDAQAPPLICATION" 
   (	"XDAQAPPLICATIONID" NUMBER(11,0), 
	"XE_XDAQEXECUTIVEID" NUMBER(11,0), 
	"CLASSNAME" VARCHAR2(50), 
	"INSTANCE" NUMBER(11,0), 
	"APPID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TCSORTCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TCSORTCONF" 
   (	"TCSORTCONFID" NUMBER(11,0), 
	"RECMUXCLKINV" RAW(18), 
	"RECMUXREGADD" RAW(18), 
	"IA9_RECMUXDELAYID" NUMBER(11,0), 
	"IA9_RECDATADELAYID" NUMBER(11,0), 
	"RECMUXCLK90" RAW(18) DEFAULT HEXTORAW(0), 
	"BOOTSCRIPT" VARCHAR2(256)
   ) ;
 
--------------------------------------------------------
--  DDL for Table RBCBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."RBCBOARD" 
   (	"RBCBOARDID" NUMBER(11,0), 
	"MASTERID" NUMBER(11,0), 
	"I2CCHANNEL" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CHAMBERLOCATION
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CHAMBERLOCATION" 
   (	"CHAMBERLOCATIONID" NUMBER(11,0), 
	"CT_CHAMBERTYPEID" NUMBER(11,0), 
	"GASCELL_GASCELLID" NUMBER(11,0), 
	"DISKORWHEEL" NUMBER(4,0), 
	"LAYER" NUMBER(4,0), 
	"SECTOR" NUMBER(4,0), 
	"SUBSECTOR" VARCHAR2(30), 
	"CHAMBERLOCATIONNAME" VARCHAR2(30), 
	"FEBZORNT" VARCHAR2(4), 
	"FEBRADORNT" VARCHAR2(4), 
	"BARRELORENDCAP" VARCHAR2(10)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TRIGGERBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TRIGGERBOARD" 
   (	"TRIGGERBOARDID" NUMBER(11,0), 
	"DCCBOARD_DCCBOARDID" NUMBER(11,0), 
	"DCCINPUTCHANNELNUM" NUMBER(11,0), 
	"DCCINPUTCONNNUM" NUMBER(11,0), 
	"TOWERTO" VARCHAR2(30), 
	"TOWERFROM" VARCHAR2(30), 
	"SECTOR" VARCHAR2(30), 
	"LOGICALASSIGNMENT" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table XDAQAPPVMECRATEACCESS
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."XDAQAPPVMECRATEACCESS" 
   (	"XDAQAPPVMECRATEACCESSID" NUMBER(11,0), 
	"CRATE_CRATEID" NUMBER(11,0), 
	"PCISLOT" NUMBER(4,0), 
	"POSINVMECHAIN" NUMBER(4,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CONFIGKEY
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CONFIGKEY" 
   (	"CONFIGKEYID" VARCHAR2(256), 
	"DESCRIPTION" VARCHAR2(256)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CCSBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CCSBOARD" 
   (	"CCSBOARDID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table SPLITTERBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."SPLITTERBOARD" 
   (	"SPLITTERBOARDID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table LINKBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."LINKBOARD" 
   (	"LINKBOARDID" NUMBER(11,0), 
	"STRIPINV" VARCHAR2(30), 
	"MASTERID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CHIP
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CHIP" 
   (	"CHIPID" NUMBER(11,0), 
	"BOARD_BOARDID" NUMBER(11,0), 
	"TYPE" VARCHAR2(30), 
	"POSITION" NUMBER(2,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table FEBCHIPCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."FEBCHIPCONF" 
   (	"FEBCHIPCONFID" NUMBER(11,0), 
	"VTH" NUMBER(11,0), 
	"VTHOFFSET" NUMBER(11,0), 
	"VMON" NUMBER(11,0), 
	"VMONOFFSET" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table INTARRAY6
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."INTARRAY6" 
   (	"INTARRAY6ID" NUMBER(11,0), 
	"VALUE0" NUMBER(11,0), 
	"VALUE1" NUMBER(11,0), 
	"VALUE2" NUMBER(11,0), 
	"VALUE3" NUMBER(11,0), 
	"VALUE4" NUMBER(11,0), 
	"VALUE5" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table INTARRAY9
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."INTARRAY9" 
   (	"INTARRAY9ID" NUMBER(11,0), 
	"VALUE0" NUMBER(11,0), 
	"VALUE1" NUMBER(11,0), 
	"VALUE2" NUMBER(11,0), 
	"VALUE3" NUMBER(11,0), 
	"VALUE4" NUMBER(11,0), 
	"VALUE5" NUMBER(11,0), 
	"VALUE6" NUMBER(11,0), 
	"VALUE7" NUMBER(11,0), 
	"VALUE8" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table GASRACK
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."GASRACK" 
   (	"GASRACKID" NUMBER(11,0), 
	"NAME" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table SORTERBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."SORTERBOARD" 
   (	"SORTERBOARDID" NUMBER(11,0), 
	"LOGICALASSIGNMENT" NUMBER(11,0), 
	"SORTERTYPE" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CHIPDISABLED
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CHIPDISABLED" 
   (	"CHIPDISABLEDID" NUMBER(11,0), 
	"CHIP_CHIPID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table SORTERCHIPCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."SORTERCHIPCONF" 
   (	"SORTERCHIPSTATICCONFID" NUMBER(11,0), 
	"BCN0DELAY" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table GASCELL
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."GASCELL" 
   (	"GASCELLID" NUMBER(11,0), 
	"GR_GASRACKID" NUMBER(11,0), 
	"INPUTMANIFOLD" VARCHAR2(4), 
	"OUTPUTMANIFOLD" VARCHAR2(4), 
	"INPUTCELLNUMBER" VARCHAR2(4), 
	"OUTPUTCELLNUMBER" VARCHAR2(4)
   ) ;
 
--------------------------------------------------------
--  DDL for Table BOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."BOARD" 
   (	"BOARDID" NUMBER(11,0), 
	"CRATE_CRATEID" NUMBER(11,0), 
	"TYPE" VARCHAR2(30), 
	"DATEOFINSTALLATION" DATE, 
	"NAME" VARCHAR2(30), 
	"LABEL" VARCHAR2(30), 
	"POSITION" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CURRENTCONFIGKEY
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CURRENTCONFIGKEY" 
   (	"KEYID" NUMBER(10,0), 
	"KEYNAME" VARCHAR2(128)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TCBACKPLANECHIPCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TCBACKPLANECHIPCONF" 
   (	"TCBACKPLANECHIPSTATICCONFID" NUMBER(11,0), 
	"BCN0DELAY" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table GASMIXTURE
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."GASMIXTURE" 
   (	"GASMIXTUREID" NUMBER(11,0), 
	"HUMIDITY" NUMBER(5,2), 
	"FREON" NUMBER(5,2), 
	"SF6" NUMBER(5,2), 
	"ISOBUTANERECIRCULATIONPERCENT" NUMBER(5,2), 
	"EXTERNALTEMPERATURE" NUMBER(5,2), 
	"ATMOSPHERICPRESSURE" NUMBER(11,2), 
	"UNIXTIME" NUMBER(14,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table STATICCONFIGURATION
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."STATICCONFIGURATION" 
   (	"STATICCONFIGURATIONID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CHIPCONFASSIGNMENT
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CHIPCONFASSIGNMENT" 
   (	"CHIPCONFASSIGNMENTID" NUMBER(11,0), 
	"SC_STATICCONFIGURATIONID" NUMBER(11,0), 
	"FIRWARE_FIRMWAREID" NUMBER(11,0), 
	"DC_DYNAMICCONFID" NUMBER(11,0), 
	"CHIP_CHIPID" NUMBER(11,0), 
	"LCK_LOCALCONFIGKEYID" NUMBER(11,0), 
	"VERSION" NUMBER(11,0), 
	"TAG" VARCHAR2(30), 
	"CREATIONDATE" TIMESTAMP (6)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TTUBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TTUBOARD" 
   (	"TTUBOARDID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table DISTRIBUTIONBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."DISTRIBUTIONBOARD" 
   (	"DISTRIBUTIONBOARDID" NUMBER(11,0), 
	"PRODUCTID" VARCHAR2(30), 
	"DATEOFINSTALLATION" DATE
   ) ;
 
--------------------------------------------------------
--  DDL for Table TCBACKPLANE
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TCBACKPLANE" 
   (	"TCBACKPLANEID" NUMBER(11,0), 
	"LOGSECTOR" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table DCCBOARD
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."DCCBOARD" 
   (	"DCCBOARDID" NUMBER(11,0), 
	"DAQID" NUMBER(11,0), 
	"FEDNUMBER" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table INTARRAY12
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."INTARRAY12" 
   (	"INTARRAY12ID" NUMBER(11,0), 
	"VALUE0" NUMBER(11,0), 
	"VALUE1" NUMBER(11,0), 
	"VALUE2" NUMBER(11,0), 
	"VALUE3" NUMBER(11,0), 
	"VALUE4" NUMBER(11,0), 
	"VALUE5" NUMBER(11,0), 
	"VALUE6" NUMBER(11,0), 
	"VALUE7" NUMBER(11,0), 
	"VALUE8" NUMBER(11,0), 
	"VALUE9" NUMBER(11,0), 
	"VALUE10" NUMBER(11,0), 
	"VALUE11" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TTUTRIGCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TTUTRIGCONF" 
   (	"TTUTRIGCONFID" NUMBER(11,0), 
	"RECMUXCLK90" RAW(6), 
	"RECMUXCLKINV" RAW(6), 
	"RECMUXREGADD" RAW(6), 
	"IA12_RECMUXDELAYID" NUMBER(11,0), 
	"IA12_RECDATADELAYID" NUMBER(11,0), 
	"TATRGDELAY" NUMBER(11,0), 
	"TATRGTOFF" NUMBER(11,0), 
	"TATRGSELECT" NUMBER(11,0), 
	"TATRGCNTRGATE" NUMBER(11,0), 
	"IA24_TASECTORDELAYID" NUMBER(11,0), 
	"IA12_TAFORCELOGIC0ID" NUMBER(11,0), 
	"IA12_TAFORCELOGIC1ID" NUMBER(11,0), 
	"IA12_TAFORCELOGIC2ID" NUMBER(11,0), 
	"IA12_TAFORCELOGIC3ID" NUMBER(11,0), 
	"IA4_TRIGCONFIGID" NUMBER(11,0), 
	"IA24_TRIGMASKID" NUMBER(11,0), 
	"IA24_TRIGFORCEID" NUMBER(11,0), 
	"TRIGMAJORITY" NUMBER(11,0), 
	"IA12_SECTORMAJORITYID" NUMBER(11,0), 
	"SECTORTRIGMASK" NUMBER(11,0), 
	"SECTORTHRESHOLD" NUMBER(11,0), 
	"TOWERTHRESHOLD" NUMBER(11,0), 
	"WHEELTHRESHOLD" NUMBER(11,0), 
	"SECTTRGSEL" NUMBER(11,0), 
	"IA16_TACONFIG2ID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table HALFSORTCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."HALFSORTCONF" 
   (	"HALFSORTCONFID" NUMBER(11,0), 
	"RECCHANENA" RAW(3), 
	"IA16_RECFASTCLKINVID" NUMBER(11,0), 
	"IA16_RECFASTCLK90ID" NUMBER(11,0), 
	"IA16_RECFASTREGADDID" NUMBER(11,0), 
	"RECFASTDATADELAY" RAW(6), 
	"RECDATADELAY" RAW(4)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CONFIGKEYASSIGNMENT
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CONFIGKEYASSIGNMENT" 
   (	"CONFIGKEYASSIGNMENTID" NUMBER(11,0), 
	"CONFIGKEY" VARCHAR2(128), 
	"LCK_LOCALCONFIGKEYID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table CHAMBERSTRIP
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."CHAMBERSTRIP" 
   (	"CHAMBERSTRIPID" NUMBER(11,0), 
	"FC_FEBCONNECTORID" NUMBER(11,0), 
	"CABLEPINNUMBER" NUMBER(3,0), 
	"CHAMBERSTRIPNUMBER" NUMBER(6,0), 
	"CMSSTRIPNUMBER" NUMBER(6,0), 
	"CMSGLOBALSTRIPNUMBER" NUMBER(6,0), 
	"CABLECHANNELNUM" NUMBER(3,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table INTARRAY18
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."INTARRAY18" 
   (	"INTARRAY18ID" NUMBER(11,0), 
	"VALUE0" NUMBER(11,0), 
	"VALUE1" NUMBER(11,0), 
	"VALUE2" NUMBER(11,0), 
	"VALUE3" NUMBER(11,0), 
	"VALUE4" NUMBER(11,0), 
	"VALUE5" NUMBER(11,0), 
	"VALUE6" NUMBER(11,0), 
	"VALUE7" NUMBER(11,0), 
	"VALUE8" NUMBER(11,0), 
	"VALUE9" NUMBER(11,0), 
	"VALUE10" NUMBER(11,0), 
	"VALUE11" NUMBER(11,0), 
	"VALUE12" NUMBER(11,0), 
	"VALUE13" NUMBER(11,0), 
	"VALUE14" NUMBER(11,0), 
	"VALUE15" NUMBER(11,0), 
	"VALUE16" NUMBER(11,0), 
	"VALUE17" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table RBCTTUCONN
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."RBCTTUCONN" 
   (	"RBCTTUCONNID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table XDAQEXECUTIVE
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."XDAQEXECUTIVE" 
   (	"XDAQEXECUTIVEID" NUMBER(11,0), 
	"HOST" VARCHAR2(50), 
	"PORT" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table BOARDBOARDCONN
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."BOARDBOARDCONN" 
   (	"BOARDBOARDCONNID" NUMBER(11,0), 
	"BOARD_COLLECTORBOARDID" NUMBER(11,0), 
	"BOARD_BOARDID" NUMBER(11,0), 
	"COLLECTORBOARDINPUTNUM" NUMBER(11,0), 
	"TYPE" VARCHAR2(30)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TBPACCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TBPACCONF" 
   (	"TBPACCONFID" NUMBER(11,0), 
	"RECMUXCLKINV" RAW(18), 
	"RECMUXREGADD" RAW(18), 
	"IA18_RECMUXDELAYID" NUMBER(11,0), 
	"IA18_RECDATADELAYID" NUMBER(11,0), 
	"RECMUXCLK90" RAW(18) DEFAULT HEXTORAW(0), 
	"BXOFCOINCIDENCE" NUMBER(11,0) DEFAULT 0, 
	"PACENA" NUMBER(11,0) DEFAULT 0, 
	"PACCONFIGID" NUMBER(11,0) DEFAULT 0
   ) ;
 
--------------------------------------------------------
--  DDL for Table I2CCBCHANNEL
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."I2CCBCHANNEL" 
   (	"I2CCBCHANNELID" NUMBER(11,0), 
	"CB_CONTROLBOARDID" NUMBER(11,0), 
	"CBCHANNEL" NUMBER(4,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table INTARRAY16
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."INTARRAY16" 
   (	"INTARRAY16ID" NUMBER(11,0), 
	"VALUE0" NUMBER(11,0), 
	"VALUE1" NUMBER(11,0), 
	"VALUE2" NUMBER(11,0), 
	"VALUE3" NUMBER(11,0), 
	"VALUE4" NUMBER(11,0), 
	"VALUE5" NUMBER(11,0), 
	"VALUE6" NUMBER(11,0), 
	"VALUE7" NUMBER(11,0), 
	"VALUE8" NUMBER(11,0), 
	"VALUE9" NUMBER(11,0), 
	"VALUE10" NUMBER(11,0), 
	"VALUE11" NUMBER(11,0), 
	"VALUE12" NUMBER(11,0), 
	"VALUE13" NUMBER(11,0), 
	"VALUE14" NUMBER(11,0), 
	"VALUE15" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table FEBBOARDDTCONTROLLED
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."FEBBOARDDTCONTROLLED" 
   (	"FEBBOARDDTCONTROLLEDID" NUMBER(11,0), 
	"BOARD_BOARDID" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  DDL for Table LINKCHIPCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."LINKCHIPCONF" 
   (	"LINKCHIPSTATICCONFID" NUMBER(11,0), 
	"TTCRXADDRESS" NUMBER(11,0), 
	"WINDOWOPEN" NUMBER(11,0), 
	"BCN0DELAY" NUMBER(11,0), 
	"LMUXINDELAY" NUMBER(11,0), 
	"RBCDELAY" NUMBER(11,0), 
	"DATATRGDELAY" NUMBER(11,0), 
	"DATADAQDELAY" NUMBER(11,0), 
	"PULSERTIMERTRGDELAY" NUMBER(11,0), 
	"WINDOWCLOSE" NUMBER(11,0), 
	"INVERTCLOCK" VARCHAR2(1), 
	"INCHANNELSENA" RAW(12)
   ) ;
 
--------------------------------------------------------
--  DDL for Table TTUFINALCONF
--------------------------------------------------------

  CREATE TABLE "CMS_RPC_CONF"."TTUFINALCONF" 
   (	"TTUFINALCONFID" NUMBER(11,0), 
	"IA6_TRIGCONFIGID" NUMBER(11,0), 
	"IA12_TRIGCONFIG2ID" NUMBER(11,0), 
	"MASKTTU" NUMBER(11,0), 
	"MASKPOINTING" NUMBER(11,0), 
	"MASKBLAST" NUMBER(11,0), 
	"DELAYTTU" NUMBER(11,0), 
	"DELAYPOINTING" NUMBER(11,0), 
	"DELAYBLAST" NUMBER(11,0), 
	"SHAPETTU" NUMBER(11,0), 
	"SHAPEPOINTING" NUMBER(11,0), 
	"SHAPEBLAST" NUMBER(11,0)
   ) ;
 
--------------------------------------------------------
--  Constraints for Table BACKPLANESORTERCONN
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."BACKPLANESORTERCONN" ADD CONSTRAINT "BACKPLANESORTERCONN_PK" PRIMARY KEY ("BACKPLANESORTERCONNID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table BOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."BOARD" ADD CONSTRAINT "BOARD_PK" PRIMARY KEY ("BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."BOARD" ADD CONSTRAINT "UNIQUE_BOARD_CRATE_POSITION" UNIQUE ("CRATE_CRATEID", "POSITION") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."BOARD" ADD CONSTRAINT "UNIQUE_BOARD_LABEL" UNIQUE ("LABEL") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."BOARD" ADD CONSTRAINT "UNIQUE_BOARD_NAME" UNIQUE ("NAME") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table BOARDBOARDCONN
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."BOARDBOARDCONN" ADD CONSTRAINT "BOARDBOARDCONN_PK" PRIMARY KEY ("BOARDBOARDCONNID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table BOARDDISABLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."BOARDDISABLED" ADD CONSTRAINT "BOARDDISABLED_UK1" UNIQUE ("BOARD_BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."BOARDDISABLED" ADD PRIMARY KEY ("BOARDDISABLEDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CCSBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CCSBOARD" ADD CONSTRAINT "CCSBOARD_PK" PRIMARY KEY ("CCSBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CHAMBER
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHAMBER" ADD CONSTRAINT "CHAMBER_PK" PRIMARY KEY ("CHAMBERID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHAMBER" MODIFY ("CHAMBERID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."CHAMBER" MODIFY ("CL_CHAMBERLOCATIONID" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table CHAMBERLOCATION
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHAMBERLOCATION" ADD CONSTRAINT "CHAMBERLOCATION_PK" PRIMARY KEY ("CHAMBERLOCATIONID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHAMBERLOCATION" MODIFY ("CHAMBERLOCATIONID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."CHAMBERLOCATION" MODIFY ("CT_CHAMBERTYPEID" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table CHAMBERSTRIP
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHAMBERSTRIP" ADD CONSTRAINT "CHAMBERSTRIP_PK" PRIMARY KEY ("CHAMBERSTRIPID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CHAMBERTYPE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHAMBERTYPE" ADD CONSTRAINT "CHAMBERTYPE_PK" PRIMARY KEY ("CHAMBERTYPEID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHAMBERTYPE" MODIFY ("CHAMBERTYPEID" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table CHIP
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHIP" ADD CONSTRAINT "CHIP_PK" PRIMARY KEY ("CHIPID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CHIPCONFASSIGNMENT
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHIPCONFASSIGNMENT" ADD CONSTRAINT "CHIPCONFASSIGNMENT_PK" PRIMARY KEY ("CHIPCONFASSIGNMENTID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHIPCONFASSIGNMENT" ADD CONSTRAINT "CHIPCONFASSIGNMENT_UK1" UNIQUE ("CHIP_CHIPID", "LCK_LOCALCONFIGKEYID") DISABLE;
 
--------------------------------------------------------
--  Constraints for Table CHIPDISABLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHIPDISABLED" ADD CONSTRAINT "CHIPDISABLED_UK1" UNIQUE ("CHIP_CHIPID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHIPDISABLED" ADD PRIMARY KEY ("CHIPDISABLEDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CONFIGKEY
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEY" MODIFY ("CONFIGKEYID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEY" ADD PRIMARY KEY ("CONFIGKEYID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CONFIGKEYASSIGNMENT
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEYASSIGNMENT" MODIFY ("CONFIGKEYASSIGNMENTID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEYASSIGNMENT" MODIFY ("CONFIGKEY" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEYASSIGNMENT" MODIFY ("LCK_LOCALCONFIGKEYID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEYASSIGNMENT" ADD PRIMARY KEY ("CONFIGKEYASSIGNMENTID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CONFIGKEY_DEPR
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEY_DEPR" ADD CONSTRAINT "PK_CONFIGKEY_DEPR" PRIMARY KEY ("INVALID_KEY") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CONTROLBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CONTROLBOARD" ADD CONSTRAINT "CONTROLBOARD_PK" PRIMARY KEY ("CONTROLBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CRATE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CRATE" ADD CONSTRAINT "CRATE_PK" PRIMARY KEY ("CRATEID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CRATE" ADD CONSTRAINT "UNIQUE_CRATE_LABEL" UNIQUE ("LABEL") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CRATE" ADD CONSTRAINT "UNIQUE_CRCATE_NAME" UNIQUE ("NAME") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CRATEDISABLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CRATEDISABLED" ADD CONSTRAINT "CRATEDISABLED_UK1" UNIQUE ("CRATE_CRATEID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CRATEDISABLED" ADD PRIMARY KEY ("CRATEDISABLEDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CSCCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CSCCHIPCONF" ADD CONSTRAINT "CSCCHIPCONF_PK" PRIMARY KEY ("CSCCHIPSTATICCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table CURRENTCONFIGKEY
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CURRENTCONFIGKEY" ADD PRIMARY KEY ("KEYID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table DCCBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DCCBOARD" ADD CONSTRAINT "DCCBOARD_PK" PRIMARY KEY ("DCCBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table DCCCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DCCCHIPCONF" ADD CONSTRAINT "DCCCHIPCONF_PK" PRIMARY KEY ("DCCCHIPSTATICCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table DISTRIBUTIONBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DISTRIBUTIONBOARD" ADD CONSTRAINT "DISTRIBUTIONBOARD_PK" PRIMARY KEY ("DISTRIBUTIONBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table DYNAMICCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DYNAMICCONF" ADD CONSTRAINT "DYNAMICCONF_PK" PRIMARY KEY ("DYNAMICCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table DYNAMICCONFDEF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DYNAMICCONFDEF" ADD CONSTRAINT "DYNAMICCONFDEF_PK" PRIMARY KEY ("DYNAMICCONFDEFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table DYNAMICPARAMDEF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DYNAMICPARAMDEF" ADD CONSTRAINT "DYNAMICPARAMDEF_PK" PRIMARY KEY ("DYNAMICPARAMDEFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table DYNAMICPARAMVALUE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DYNAMICPARAMVALUE" ADD CONSTRAINT "DYNAMICPARAMVALUE_PK" PRIMARY KEY ("DYNAMICPARAMVALUEID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table FEB
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEB" ADD CONSTRAINT "FEB_PK" PRIMARY KEY ("FEBID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table FEBBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBBOARD" ADD CONSTRAINT "FEBBOARD_PK" PRIMARY KEY ("FEBBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table FEBBOARDDTCONTROLLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBBOARDDTCONTROLLED" ADD CONSTRAINT "FEBBOARDDTCONTROLLEDID_UK1" UNIQUE ("BOARD_BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."FEBBOARDDTCONTROLLED" MODIFY ("FEBBOARDDTCONTROLLEDID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."FEBBOARDDTCONTROLLED" MODIFY ("BOARD_BOARDID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."FEBBOARDDTCONTROLLED" ADD PRIMARY KEY ("FEBBOARDDTCONTROLLEDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table FEBCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBCHIPCONF" ADD CONSTRAINT "FEBCHIPCONF_PK" PRIMARY KEY ("FEBCHIPCONFID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."FEBCHIPCONF" MODIFY ("VTH" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."FEBCHIPCONF" MODIFY ("VTHOFFSET" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."FEBCHIPCONF" MODIFY ("VMON" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."FEBCHIPCONF" MODIFY ("VMONOFFSET" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table FEBCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBCONF" ADD CONSTRAINT "FEBCONF_PK" PRIMARY KEY ("FEBCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table FEBCONNECTOR
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBCONNECTOR" ADD CONSTRAINT "FEBCONNECTOR_PK" PRIMARY KEY ("FEBCONNECTORID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table FEBLOCATION
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBLOCATION" ADD CONSTRAINT "FEBLOCATION_PK" PRIMARY KEY ("FEBLOCATIONID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table FIRMWARE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FIRMWARE" ADD CONSTRAINT "FIRMWARE_PK" PRIMARY KEY ("FIRMWAREID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table GASCELL
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."GASCELL" ADD CONSTRAINT "GASCELL_PK" PRIMARY KEY ("GASCELLID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."GASCELL" MODIFY ("GASCELLID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."GASCELL" MODIFY ("GR_GASRACKID" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table GASMIXTURE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."GASMIXTURE" ADD CONSTRAINT "GASMIXTURE_PK" PRIMARY KEY ("GASMIXTUREID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."GASMIXTURE" MODIFY ("GASMIXTUREID" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table GASRACK
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."GASRACK" ADD CONSTRAINT "GASRACK_PK" PRIMARY KEY ("GASRACKID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."GASRACK" MODIFY ("GASRACKID" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table HALFSORTCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" ADD CONSTRAINT "HALFSORTCONF_PK" PRIMARY KEY ("HALFSORTCONFID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" MODIFY ("HALFSORTCONFID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" MODIFY ("RECCHANENA" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" MODIFY ("IA16_RECFASTCLKINVID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" MODIFY ("IA16_RECFASTCLK90ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" MODIFY ("IA16_RECFASTREGADDID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" MODIFY ("RECFASTDATADELAY" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" MODIFY ("RECDATADELAY" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table HOBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."HOBOARD" ADD CONSTRAINT "HOBOARD_PK" PRIMARY KEY ("HOBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table I2CCBCHANNEL
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."I2CCBCHANNEL" ADD CONSTRAINT "I2CCBCHANNEL_PK" PRIMARY KEY ("I2CCBCHANNELID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."I2CCBCHANNEL" ADD CONSTRAINT "UNIQUE_I2CCBCHANNEL" UNIQUE ("CB_CONTROLBOARDID", "I2CCBCHANNELID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table INTARRAY12
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" ADD CONSTRAINT "INTARRAY12ID_PK" PRIMARY KEY ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("INTARRAY12ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE0" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE1" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE2" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE3" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE4" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE5" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE6" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE7" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE8" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE9" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE10" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY12" MODIFY ("VALUE11" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table INTARRAY16
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" ADD CONSTRAINT "INTARRAY16ID_PK" PRIMARY KEY ("INTARRAY16ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("INTARRAY16ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE0" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE1" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE2" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE3" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE4" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE5" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE6" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE7" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE8" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE9" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE10" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE11" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE12" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE13" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE14" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY16" MODIFY ("VALUE15" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table INTARRAY18
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."INTARRAY18" ADD CONSTRAINT "INTARRAY18ID_PK" PRIMARY KEY ("INTARRAY18ID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table INTARRAY24
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" ADD CONSTRAINT "INTARRAY24ID_PK" PRIMARY KEY ("INTARRAY24ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("INTARRAY24ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE0" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE1" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE2" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE3" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE4" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE5" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE6" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE7" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE8" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE9" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE10" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE11" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE12" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE13" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE14" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE15" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE16" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE17" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE18" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE19" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE20" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE21" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE22" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY24" MODIFY ("VALUE23" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table INTARRAY4
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."INTARRAY4" ADD CONSTRAINT "INTARRAY4ID_PK" PRIMARY KEY ("INTARRAY4ID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table INTARRAY6
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."INTARRAY6" ADD CONSTRAINT "INTARRAY6ID_PK" PRIMARY KEY ("INTARRAY6ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY6" MODIFY ("INTARRAY6ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY6" MODIFY ("VALUE0" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY6" MODIFY ("VALUE1" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY6" MODIFY ("VALUE2" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY6" MODIFY ("VALUE3" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY6" MODIFY ("VALUE4" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."INTARRAY6" MODIFY ("VALUE5" NOT NULL ENABLE);
 
--------------------------------------------------------
--  Constraints for Table INTARRAY9
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."INTARRAY9" ADD CONSTRAINT "INTARRAY9ID_PK" PRIMARY KEY ("INTARRAY9ID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table LINKBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LINKBOARD" ADD CONSTRAINT "LINKBOARD_PK" PRIMARY KEY ("LINKBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table LINKCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LINKCHIPCONF" ADD CONSTRAINT "LINKCHIPCONF_PK" PRIMARY KEY ("LINKCHIPSTATICCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table LINKDISABLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LINKDISABLED" ADD CONSTRAINT "LINKDISABLED_UK1" UNIQUE ("BBC_BOARDBOARDCONNID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."LINKDISABLED" MODIFY ("LINKDISABLEDID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."LINKDISABLED" MODIFY ("BBC_BOARDBOARDCONNID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."LINKDISABLED" ADD PRIMARY KEY ("LINKDISABLEDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table LINKTBCONN
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LINKTBCONN" ADD CONSTRAINT "LINKTBCONN_PK" PRIMARY KEY ("LINKTBCONNID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table LOCALCONFIGKEY
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LOCALCONFIGKEY" ADD CONSTRAINT "LOCALCONFIGKEY_PK" PRIMARY KEY ("LOCALCONFIGKEYID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table RBCBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."RBCBOARD" ADD CONSTRAINT "RBCBOARD_PK" PRIMARY KEY ("RBCBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table RBCCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."RBCCHIPCONF" ADD CONSTRAINT "RBCCHIPCONF_PK" PRIMARY KEY ("RBCCHIPSTATICCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table RBCTTUCONN
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."RBCTTUCONN" ADD CONSTRAINT "RBCTTUCONN_PK" PRIMARY KEY ("RBCTTUCONNID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table SORTERBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."SORTERBOARD" ADD CONSTRAINT "SORTERBOARD_PK" PRIMARY KEY ("SORTERBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table SORTERCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."SORTERCHIPCONF" ADD CONSTRAINT "SORTERCHIPCONF_PK" PRIMARY KEY ("SORTERCHIPSTATICCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table SPLITTERBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."SPLITTERBOARD" ADD CONSTRAINT "SPLITTERBOARD_PK" PRIMARY KEY ("SPLITTERBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table STATICCONFIGURATION
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."STATICCONFIGURATION" ADD CONSTRAINT "STATICCONFIGURATION_PK" PRIMARY KEY ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TBGBSORTCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TBGBSORTCONF" MODIFY ("RECMUXCLK90" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TBGBSORTCONF" ADD CONSTRAINT "TBTBGBSORTCONF_PK" PRIMARY KEY ("TBGBSORTCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TBPACCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TBPACCONF" MODIFY ("RECMUXCLK90" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TBPACCONF" MODIFY ("BXOFCOINCIDENCE" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TBPACCONF" MODIFY ("PACENA" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TBPACCONF" MODIFY ("PACCONFIGID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TBPACCONF" ADD CONSTRAINT "TBPACCONF_PK" PRIMARY KEY ("TBPACCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TBRMBCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TBRMBCONF" MODIFY ("RECMUXCLK90" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TBRMBCONF" ADD CONSTRAINT "TBRMBCONF_PK" PRIMARY KEY ("TBRMBCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TCBACKPLANE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TCBACKPLANE" ADD CONSTRAINT "TCBACKPLANE_PK" PRIMARY KEY ("TCBACKPLANEID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TCBACKPLANECHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TCBACKPLANECHIPCONF" ADD CONSTRAINT "TCBACKPLANECHIPCONF_PK" PRIMARY KEY ("TCBACKPLANECHIPSTATICCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TCSORTCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TCSORTCONF" MODIFY ("RECMUXCLK90" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TCSORTCONF" ADD CONSTRAINT "TBTCSORTCONF_PK" PRIMARY KEY ("TCSORTCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TRIGGERBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TRIGGERBOARD" ADD CONSTRAINT "TRIGGERBOARD_PK" PRIMARY KEY ("TRIGGERBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TTUBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TTUBOARD" ADD CONSTRAINT "TTUBOARD_PK" PRIMARY KEY ("TTUBOARDID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TTUFINALCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("TTUFINALCONFID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("IA6_TRIGCONFIGID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("IA12_TRIGCONFIG2ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("MASKTTU" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("MASKPOINTING" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("MASKBLAST" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("DELAYTTU" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("DELAYPOINTING" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("DELAYBLAST" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("SHAPETTU" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("SHAPEPOINTING" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" MODIFY ("SHAPEBLAST" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" ADD CONSTRAINT "TTUFINALCONF_PK" PRIMARY KEY ("TTUFINALCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table TTUTRIGCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("TTUTRIGCONFID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("RECMUXCLK90" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("RECMUXCLKINV" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("RECMUXREGADD" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA12_RECMUXDELAYID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA12_RECDATADELAYID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("TATRGDELAY" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("TATRGTOFF" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("TATRGSELECT" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("TATRGCNTRGATE" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA24_TASECTORDELAYID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA12_TAFORCELOGIC0ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA12_TAFORCELOGIC1ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA12_TAFORCELOGIC2ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA12_TAFORCELOGIC3ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA4_TRIGCONFIGID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA24_TRIGMASKID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA24_TRIGFORCEID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("TRIGMAJORITY" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA12_SECTORMAJORITYID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("SECTORTRIGMASK" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("SECTORTHRESHOLD" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("TOWERTHRESHOLD" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("WHEELTHRESHOLD" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("SECTTRGSEL" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" MODIFY ("IA16_TACONFIG2ID" NOT NULL ENABLE);
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "TTUTRIGCONF_PK" PRIMARY KEY ("TTUTRIGCONFID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table XDAQAPPLBOXACCESS
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPLBOXACCESS" ADD CONSTRAINT "UNIQUE_XDAQAPPLBOXACCESS" UNIQUE ("CCS_CCSBOARDID", "FECPOSITION") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPLBOXACCESS" ADD CONSTRAINT "XDAQAPPLBOXACCESS_PK" PRIMARY KEY ("XDAQAPPLBOXACCESSID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table XDAQAPPLICATION
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPLICATION" ADD CONSTRAINT "UNIQUE_XDAQAPPLICATION" UNIQUE ("XE_XDAQEXECUTIVEID", "CLASSNAME", "INSTANCE") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPLICATION" ADD CONSTRAINT "UNIQUE_XDAQAPPLICATION_1" UNIQUE ("XE_XDAQEXECUTIVEID", "APPID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPLICATION" ADD CONSTRAINT "XDAQAPPLICATION_PK" PRIMARY KEY ("XDAQAPPLICATIONID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table XDAQAPPVMECRATEACCESS
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPVMECRATEACCESS" ADD CONSTRAINT "UNIQUE_XDAQAPPVMECRATEACCESS" UNIQUE ("CRATE_CRATEID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPVMECRATEACCESS" ADD CONSTRAINT "XDAQAPPVMECRATEACCESS_PK" PRIMARY KEY ("XDAQAPPVMECRATEACCESSID") ENABLE;
 
--------------------------------------------------------
--  Constraints for Table XDAQEXECUTIVE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."XDAQEXECUTIVE" ADD CONSTRAINT "UNIQUE_XDAQEXECUTIVE" UNIQUE ("HOST", "PORT") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."XDAQEXECUTIVE" ADD CONSTRAINT "XDAQEXECUTIVE_PK" PRIMARY KEY ("XDAQEXECUTIVEID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table BACKPLANESORTERCONN
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."BACKPLANESORTERCONN" ADD CONSTRAINT "SORTERBOARD_FK1" FOREIGN KEY ("SORTERBOARD_SORTERBOARDID")
	  REFERENCES "CMS_RPC_CONF"."SORTERBOARD" ("SORTERBOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."BACKPLANESORTERCONN" ADD CONSTRAINT "TCBACKPLANE_FK1" FOREIGN KEY ("TCBP_TCBACKPLANEID")
	  REFERENCES "CMS_RPC_CONF"."TCBACKPLANE" ("TCBACKPLANEID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table BOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."BOARD" ADD CONSTRAINT "CRATE_FK1" FOREIGN KEY ("CRATE_CRATEID")
	  REFERENCES "CMS_RPC_CONF"."CRATE" ("CRATEID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table BOARDBOARDCONN
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."BOARDBOARDCONN" ADD CONSTRAINT "BOARD_FK11" FOREIGN KEY ("BOARD_BOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."BOARDBOARDCONN" ADD CONSTRAINT "BOARD_FK15" FOREIGN KEY ("BOARD_COLLECTORBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table BOARDDISABLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."BOARDDISABLED" ADD FOREIGN KEY ("BOARD_BOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CCSBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CCSBOARD" ADD CONSTRAINT "BOARD_FK2" FOREIGN KEY ("CCSBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CHAMBER
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHAMBER" ADD CONSTRAINT "CHAMBERLOCATION_FK1" FOREIGN KEY ("CL_CHAMBERLOCATIONID")
	  REFERENCES "CMS_RPC_CONF"."CHAMBERLOCATION" ("CHAMBERLOCATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CHAMBERLOCATION
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHAMBERLOCATION" ADD CONSTRAINT "CHAMBERTYPE_FK1" FOREIGN KEY ("CT_CHAMBERTYPEID")
	  REFERENCES "CMS_RPC_CONF"."CHAMBERTYPE" ("CHAMBERTYPEID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHAMBERLOCATION" ADD CONSTRAINT "GASCELL_FK1" FOREIGN KEY ("GASCELL_GASCELLID")
	  REFERENCES "CMS_RPC_CONF"."GASCELL" ("GASCELLID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CHAMBERSTRIP
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHAMBERSTRIP" ADD CONSTRAINT "FEBCONNECTOR_FK1" FOREIGN KEY ("FC_FEBCONNECTORID")
	  REFERENCES "CMS_RPC_CONF"."FEBCONNECTOR" ("FEBCONNECTORID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CHIP
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHIP" ADD CONSTRAINT "BOARD_FK1" FOREIGN KEY ("BOARD_BOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CHIPCONFASSIGNMENT
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHIPCONFASSIGNMENT" ADD CONSTRAINT "CHIP_FK1" FOREIGN KEY ("CHIP_CHIPID")
	  REFERENCES "CMS_RPC_CONF"."CHIP" ("CHIPID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHIPCONFASSIGNMENT" ADD CONSTRAINT "DYNAMICCONF_FK2" FOREIGN KEY ("DC_DYNAMICCONFID")
	  REFERENCES "CMS_RPC_CONF"."DYNAMICCONF" ("DYNAMICCONFID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHIPCONFASSIGNMENT" ADD CONSTRAINT "FIRMWARE_FK1" FOREIGN KEY ("FIRWARE_FIRMWAREID")
	  REFERENCES "CMS_RPC_CONF"."FIRMWARE" ("FIRMWAREID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CHIPCONFASSIGNMENT" ADD CONSTRAINT "STATICCONFIGURATION_FK1" FOREIGN KEY ("SC_STATICCONFIGURATIONID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CHIPDISABLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CHIPDISABLED" ADD FOREIGN KEY ("CHIP_CHIPID")
	  REFERENCES "CMS_RPC_CONF"."CHIP" ("CHIPID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CONFIGKEYASSIGNMENT
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEYASSIGNMENT" ADD FOREIGN KEY ("CONFIGKEY")
	  REFERENCES "CMS_RPC_CONF"."CONFIGKEY" ("CONFIGKEYID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CONFIGKEYASSIGNMENT" ADD FOREIGN KEY ("LCK_LOCALCONFIGKEYID")
	  REFERENCES "CMS_RPC_CONF"."LOCALCONFIGKEY" ("LOCALCONFIGKEYID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CONTROLBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CONTROLBOARD" ADD CONSTRAINT "BOARD_FK3" FOREIGN KEY ("CONTROLBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."CONTROLBOARD" ADD CONSTRAINT "CCSBOARD_FK1" FOREIGN KEY ("CCSBOARD_CCSBOARDID")
	  REFERENCES "CMS_RPC_CONF"."CCSBOARD" ("CCSBOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CRATEDISABLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CRATEDISABLED" ADD FOREIGN KEY ("CRATE_CRATEID")
	  REFERENCES "CMS_RPC_CONF"."CRATE" ("CRATEID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table CSCCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."CSCCHIPCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK6" FOREIGN KEY ("CSCCHIPSTATICCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table DCCBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DCCBOARD" ADD CONSTRAINT "BOARD_FK6" FOREIGN KEY ("DCCBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table DCCCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DCCCHIPCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK5" FOREIGN KEY ("DCCCHIPSTATICCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table DYNAMICPARAMDEF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DYNAMICPARAMDEF" ADD CONSTRAINT "DYNAMICCONFDEF_FK2" FOREIGN KEY ("DCD_DYNAMICCONFDEFID")
	  REFERENCES "CMS_RPC_CONF"."DYNAMICCONFDEF" ("DYNAMICCONFDEFID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table DYNAMICPARAMVALUE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."DYNAMICPARAMVALUE" ADD CONSTRAINT "DYNAMICCONF_FK1" FOREIGN KEY ("DC_DYNAMICCONFID")
	  REFERENCES "CMS_RPC_CONF"."DYNAMICCONF" ("DYNAMICCONFID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."DYNAMICPARAMVALUE" ADD CONSTRAINT "DYNAMICPARAMDEF_FK1" FOREIGN KEY ("DPD_DYNAMICPARAMDEFID")
	  REFERENCES "CMS_RPC_CONF"."DYNAMICPARAMDEF" ("DYNAMICPARAMDEFID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table FEB
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEB" ADD CONSTRAINT "DISTRIBUTIONBOARD_FEB_FK1" FOREIGN KEY ("DB_DISTRIBUTIONBOARDID")
	  REFERENCES "CMS_RPC_CONF"."DISTRIBUTIONBOARD" ("DISTRIBUTIONBOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."FEB" ADD CONSTRAINT "FEBLOCATION_FEB_FK1" FOREIGN KEY ("FL_FEBLOCATIONID")
	  REFERENCES "CMS_RPC_CONF"."FEBLOCATION" ("FEBLOCATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table FEBBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBBOARD" ADD CONSTRAINT "BOARD_FK14" FOREIGN KEY ("FEBBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."FEBBOARD" ADD CONSTRAINT "FEBLOCATION_FK3" FOREIGN KEY ("FL_FEBLOCATIONID")
	  REFERENCES "CMS_RPC_CONF"."FEBLOCATION" ("FEBLOCATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table FEBBOARDDTCONTROLLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBBOARDDTCONTROLLED" ADD FOREIGN KEY ("BOARD_BOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table FEBCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBCHIPCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK19" FOREIGN KEY ("FEBCHIPCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table FEBCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBCONF" ADD CONSTRAINT "FEBLOCATION_FK2" FOREIGN KEY ("FL_FEBLOCATIONID")
	  REFERENCES "CMS_RPC_CONF"."FEBLOCATION" ("FEBLOCATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table FEBCONNECTOR
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBCONNECTOR" ADD CONSTRAINT "FEBLOCATION_FK1" FOREIGN KEY ("FL_FEBLOCATIONID")
	  REFERENCES "CMS_RPC_CONF"."FEBLOCATION" ("FEBLOCATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table FEBLOCATION
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FEBLOCATION" ADD CONSTRAINT "CHAMBERLOCATION_FK2" FOREIGN KEY ("CL_CHAMBERLOCATIONID")
	  REFERENCES "CMS_RPC_CONF"."CHAMBERLOCATION" ("CHAMBERLOCATIONID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."FEBLOCATION" ADD CONSTRAINT "I2CCBCHANNEL_FEBLOCATION_FK1" FOREIGN KEY ("CBC_I2CCBCHANNELID")
	  REFERENCES "CMS_RPC_CONF"."I2CCBCHANNEL" ("I2CCBCHANNELID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."FEBLOCATION" ADD CONSTRAINT "LINKBOARD_FK1" FOREIGN KEY ("LB_LINKBOARDID")
	  REFERENCES "CMS_RPC_CONF"."LINKBOARD" ("LINKBOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table FIRMWARE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."FIRMWARE" ADD CONSTRAINT "DYNAMICCONFDEF_FK1" FOREIGN KEY ("DCD_DYNAMICCONFDEFID")
	  REFERENCES "CMS_RPC_CONF"."DYNAMICCONFDEF" ("DYNAMICCONFDEFID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table GASCELL
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."GASCELL" ADD CONSTRAINT "GASRACK_FK1" FOREIGN KEY ("GR_GASRACKID")
	  REFERENCES "CMS_RPC_CONF"."GASRACK" ("GASRACKID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table HALFSORTCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" ADD CONSTRAINT "INTARRAY16_FK1" FOREIGN KEY ("IA16_RECFASTCLKINVID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY16" ("INTARRAY16ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" ADD CONSTRAINT "INTARRAY16_FK2" FOREIGN KEY ("IA16_RECFASTCLK90ID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY16" ("INTARRAY16ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" ADD CONSTRAINT "INTARRAY16_FK3" FOREIGN KEY ("IA16_RECFASTREGADDID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY16" ("INTARRAY16ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."HALFSORTCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK15" FOREIGN KEY ("HALFSORTCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table HOBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."HOBOARD" ADD CONSTRAINT "BOARD_FK10" FOREIGN KEY ("HOBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table I2CCBCHANNEL
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."I2CCBCHANNEL" ADD CONSTRAINT "CONTROLBOARD_I2CCBCHANNEL_FK1" FOREIGN KEY ("CB_CONTROLBOARDID")
	  REFERENCES "CMS_RPC_CONF"."CONTROLBOARD" ("CONTROLBOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table LINKBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LINKBOARD" ADD CONSTRAINT "BOARD_FK9" FOREIGN KEY ("LINKBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."LINKBOARD" ADD CONSTRAINT "LINKBOARD_FK2" FOREIGN KEY ("MASTERID")
	  REFERENCES "CMS_RPC_CONF"."LINKBOARD" ("LINKBOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table LINKCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LINKCHIPCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK2" FOREIGN KEY ("LINKCHIPSTATICCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table LINKDISABLED
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LINKDISABLED" ADD FOREIGN KEY ("BBC_BOARDBOARDCONNID")
	  REFERENCES "CMS_RPC_CONF"."BOARDBOARDCONN" ("BOARDBOARDCONNID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table LINKTBCONN
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."LINKTBCONN" ADD CONSTRAINT "BOARDBOARDCONN_FK1" FOREIGN KEY ("LINKTBCONNID")
	  REFERENCES "CMS_RPC_CONF"."BOARDBOARDCONN" ("BOARDBOARDCONNID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table RBCBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."RBCBOARD" ADD CONSTRAINT "BOARD_FK13" FOREIGN KEY ("RBCBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."RBCBOARD" ADD CONSTRAINT "RBCBOARD_FK1" FOREIGN KEY ("MASTERID")
	  REFERENCES "CMS_RPC_CONF"."RBCBOARD" ("RBCBOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table RBCCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."RBCCHIPCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK16" FOREIGN KEY ("RBCCHIPSTATICCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table RBCTTUCONN
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."RBCTTUCONN" ADD CONSTRAINT "BOARDBOARDCONN_FK2" FOREIGN KEY ("RBCTTUCONNID")
	  REFERENCES "CMS_RPC_CONF"."BOARDBOARDCONN" ("BOARDBOARDCONNID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table SORTERBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."SORTERBOARD" ADD CONSTRAINT "BOARD_FK5" FOREIGN KEY ("SORTERBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table SORTERCHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."SORTERCHIPCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK7" FOREIGN KEY ("SORTERCHIPSTATICCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table SPLITTERBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."SPLITTERBOARD" ADD CONSTRAINT "BOARD_FK8" FOREIGN KEY ("SPLITTERBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TBGBSORTCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TBGBSORTCONF" ADD CONSTRAINT "INTARRAY4_FK3" FOREIGN KEY ("IA4_RECMUXDELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY4" ("INTARRAY4ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TBGBSORTCONF" ADD CONSTRAINT "INTARRAY4_FK4" FOREIGN KEY ("IA4_RECDATADELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY4" ("INTARRAY4ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TBGBSORTCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK13" FOREIGN KEY ("TBGBSORTCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TBPACCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TBPACCONF" ADD CONSTRAINT "INTARRAY18_FK3" FOREIGN KEY ("IA18_RECMUXDELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY18" ("INTARRAY18ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TBPACCONF" ADD CONSTRAINT "INTARRAY18_FK4" FOREIGN KEY ("IA18_RECDATADELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY18" ("INTARRAY18ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TBPACCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK11" FOREIGN KEY ("TBPACCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TBRMBCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TBRMBCONF" ADD CONSTRAINT "INTARRAY18_FK5" FOREIGN KEY ("IA18_RECMUXDELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY18" ("INTARRAY18ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TBRMBCONF" ADD CONSTRAINT "INTARRAY18_FK6" FOREIGN KEY ("IA18_RECDATADELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY18" ("INTARRAY18ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TBRMBCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK12" FOREIGN KEY ("TBRMBCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TCBACKPLANE
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TCBACKPLANE" ADD CONSTRAINT "BOARD_FK4" FOREIGN KEY ("TCBACKPLANEID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TCBACKPLANECHIPCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TCBACKPLANECHIPCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK3" FOREIGN KEY ("TCBACKPLANECHIPSTATICCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TCSORTCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TCSORTCONF" ADD CONSTRAINT "INTARRAY9_FK3" FOREIGN KEY ("IA9_RECMUXDELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY9" ("INTARRAY9ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TCSORTCONF" ADD CONSTRAINT "INTARRAY9_FK4" FOREIGN KEY ("IA9_RECDATADELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY9" ("INTARRAY9ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TCSORTCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK14" FOREIGN KEY ("TCSORTCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TRIGGERBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TRIGGERBOARD" ADD CONSTRAINT "BOARD_FK7" FOREIGN KEY ("TRIGGERBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TRIGGERBOARD" ADD CONSTRAINT "DCCBOARD_FK1" FOREIGN KEY ("DCCBOARD_DCCBOARDID")
	  REFERENCES "CMS_RPC_CONF"."DCCBOARD" ("DCCBOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TTUBOARD
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TTUBOARD" ADD CONSTRAINT "BOARD_FK12" FOREIGN KEY ("TTUBOARDID")
	  REFERENCES "CMS_RPC_CONF"."BOARD" ("BOARDID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TTUFINALCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" ADD CONSTRAINT "INTARRAY12_FK8" FOREIGN KEY ("IA12_TRIGCONFIG2ID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY12" ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" ADD CONSTRAINT "INTARRAY6_FK1" FOREIGN KEY ("IA6_TRIGCONFIGID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY6" ("INTARRAY6ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUFINALCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK18" FOREIGN KEY ("TTUFINALCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table TTUTRIGCONF
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY12_FK1" FOREIGN KEY ("IA12_RECMUXDELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY12" ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY12_FK2" FOREIGN KEY ("IA12_RECDATADELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY12" ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY12_FK3" FOREIGN KEY ("IA12_TAFORCELOGIC0ID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY12" ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY12_FK4" FOREIGN KEY ("IA12_TAFORCELOGIC1ID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY12" ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY12_FK5" FOREIGN KEY ("IA12_TAFORCELOGIC2ID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY12" ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY12_FK6" FOREIGN KEY ("IA12_TAFORCELOGIC3ID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY12" ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY12_FK7" FOREIGN KEY ("IA12_SECTORMAJORITYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY12" ("INTARRAY12ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY16_FK4" FOREIGN KEY ("IA16_TACONFIG2ID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY16" ("INTARRAY16ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY24_FK1" FOREIGN KEY ("IA24_TASECTORDELAYID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY24" ("INTARRAY24ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY24_FK2" FOREIGN KEY ("IA24_TRIGMASKID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY24" ("INTARRAY24ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY24_FK3" FOREIGN KEY ("IA24_TRIGFORCEID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY24" ("INTARRAY24ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "INTARRAY4_FK5" FOREIGN KEY ("IA4_TRIGCONFIGID")
	  REFERENCES "CMS_RPC_CONF"."INTARRAY4" ("INTARRAY4ID") ENABLE;
 
  ALTER TABLE "CMS_RPC_CONF"."TTUTRIGCONF" ADD CONSTRAINT "STATICCONFIGURATION_FK17" FOREIGN KEY ("TTUTRIGCONFID")
	  REFERENCES "CMS_RPC_CONF"."STATICCONFIGURATION" ("STATICCONFIGURATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table XDAQAPPLBOXACCESS
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPLBOXACCESS" ADD CONSTRAINT "XDAQAPPLICATION_FK1" FOREIGN KEY ("XDAQAPPLBOXACCESSID")
	  REFERENCES "CMS_RPC_CONF"."XDAQAPPLICATION" ("XDAQAPPLICATIONID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table XDAQAPPLICATION
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPLICATION" ADD CONSTRAINT "XDAQEXECUTIVE_FK1" FOREIGN KEY ("XE_XDAQEXECUTIVEID")
	  REFERENCES "CMS_RPC_CONF"."XDAQEXECUTIVE" ("XDAQEXECUTIVEID") ENABLE;
 
--------------------------------------------------------
--  Ref Constraints for Table XDAQAPPVMECRATEACCESS
--------------------------------------------------------

  ALTER TABLE "CMS_RPC_CONF"."XDAQAPPVMECRATEACCESS" ADD CONSTRAINT "XDAQAPPLICATION_FK2" FOREIGN KEY ("XDAQAPPVMECRATEACCESSID")
	  REFERENCES "CMS_RPC_CONF"."XDAQAPPLICATION" ("XDAQAPPLICATIONID") ENABLE;
 
--------------------------------------------------------
--  DDL for View CHAMBERLOCATIONFEBCONNECTOR
--------------------------------------------------------

  CREATE OR REPLACE VIEW "CMS_RPC_CONF"."CHAMBERLOCATIONFEBCONNECTOR" ("CHAMBERLOCATIONID", "CT_CHAMBERTYPEID", "GASLINE_GASLINEID", "DISKORWHEEL", "LAYER", "SECTOR", "SUBSECTOR", "CHAMBERLOCATIONNAME", "FEBZORNT", "FEBRADORNT", "BARRELORENDCAP", "FEBLOCATIONID", "LB_LINKBOARDID", "DB_DISTRIBUTIONBOARDID", "CL_CHAMBERLOCATIONID", "FEBLOCALETAPARTITION", "FEBCMSETAPARTITION", "POSINLOCALETAPARTITION", "POSINCMSETAPARTITION", "I2CLOCALNUMBER", "FEBCONNECTORID", "FL_FEBLOCATIONID", "LINKBOARDINPUTNUM", "FEBCONNECTORNUM") AS 
  select * 
from chamberlocation c left outer join feblocation fl on c.chamberlocationid = fl.cl_chamberlocationid
left outer join febconnector fc on fc.fl_feblocationid = fl.feblocationid

 ;
 
--------------------------------------------------------
--  DDL for View CHAMBERLOCATIONI2CCBCHANNEL
--------------------------------------------------------

  CREATE OR REPLACE VIEW "CMS_RPC_CONF"."CHAMBERLOCATIONI2CCBCHANNEL" ("CHAMBERLOCATIONID", "CT_CHAMBERTYPEID", "GASLINE_GASLINEID", "DISKORWHEEL", "LAYER", "SECTOR", "SUBSECTOR", "CHAMBERLOCATIONNAME", "FEBZORNT", "FEBRADORNT", "BARRELORENDCAP", "FEBLOCATIONID", "LB_LINKBOARDID", "CL_CHAMBERLOCATIONID", "CBC_I2CCBCHANNELID", "FEBLOCALETAPARTITION", "FEBCMSETAPARTITION", "POSINLOCALETAPARTITION", "POSINCMSETAPARTITION", "I2CLOCALNUMBER", "I2CCBCHANNELID", "CB_CONTROLBOARDID", "CBCHANNEL") AS 
  select * from chamberlocation cl
join feblocation fl on cl.chamberlocationid = fl.cl_chamberlocationid
join i2ccbchannel i on fl.cbc_i2ccbchannelid = i.i2ccbchannelid

 ;
 