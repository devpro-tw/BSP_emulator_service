# ---------------------------------------------------------------------------
!if !$d(BCB)
BCB = $(MAKEDIR)\..
!endif

# ---------------------------------------------------------------------------
# IDE SECTION
# ---------------------------------------------------------------------------
# The following section of the project makefile is managed by the BCB IDE.
# It is recommended to use the IDE to change any of the values in this
# section.
# ---------------------------------------------------------------------------

VERSION = BCB.06.00
# ---------------------------------------------------------------------------
PROJECT = bin\EmulatorServiceProject.dll
OBJFILES = obj\EmulatorServiceProject.obj obj\EmulatorService_TLB.obj \
    obj\EmulatorServiceProject_ATL.obj obj\EmulatorManagerImpl.obj \
    obj\EmuScreenUtil.obj obj\EmuServiceUtil.obj obj\EStepDebug.obj \
    obj\PComUnit.obj obj\ServiceUtil.obj obj\APUtil.obj obj\CustomService.obj \
    obj\ECLEmulator.obj obj\EhllapiError.obj obj\EhllapiImpl.obj \
    obj\EmuInfoBase.obj obj\base64.obj
RESFILES = EmulatorServiceProject.res EmulatorService.tlb
MAINSOURCE = EmulatorServiceProject.cpp
RESDEPEN = $(RESFILES) EStepDebug.dfm
LIBFILES = 
IDLFILES = 
IDLGENFILES = 
LIBRARIES = vclnp.lib indy.lib dbxcds.lib dclocx.lib soaprtl.lib bcbie.lib nmfast.lib \
    dbexpress.lib inetdbxpress.lib inetdbbde.lib bcbsmp.lib dsnapcon.lib \
    dsnapcrba.lib visualdbclx.lib teeqr.lib dss.lib teedb.lib teeui.lib \
    vcldbx.lib qrpt.lib bdecds.lib cds.lib ibxpress.lib adortl.lib tee.lib \
    vclx.lib bdertl.lib vcldb.lib dbrtl.lib vcl.lib rtl.lib
PACKAGES = vcl.bpi rtl.bpi vclx.bpi dbrtl.bpi adortl.bpi vcldb.bpi bdertl.bpi \
    vcldbx.bpi dsnap.bpi cds.bpi bdecds.bpi visualclx.bpi visualdbclx.bpi \
    dsnapcrba.bpi dsnapcon.bpi bcbsmp.bpi vclie.bpi xmlrtl.bpi inet.bpi \
    inetdbbde.bpi inetdbxpress.bpi inetdb.bpi webdsnap.bpi bcbie.bpi \
    websnap.bpi soaprtl.bpi dclocx.bpi dbexpress.bpi dbxcds.bpi \
    bcb2kaxserver.bpi qrpt.bpi indy.bpi ibxpress.bpi nmfast.bpi dss.bpi \
    teeui.bpi teedb.bpi tee.bpi teeqr.bpi
SPARELIBS = rtl.lib vcl.lib dbrtl.lib vcldb.lib bdertl.lib vclx.lib tee.lib adortl.lib \
    ibxpress.lib cds.lib bdecds.lib qrpt.lib vcldbx.lib teeui.lib teedb.lib \
    dss.lib teeqr.lib visualdbclx.lib dsnapcrba.lib dsnapcon.lib bcbsmp.lib \
    inetdbbde.lib inetdbxpress.lib dbexpress.lib nmfast.lib bcbie.lib \
    soaprtl.lib dclocx.lib dbxcds.lib indy.lib
DEFFILE = 
OTHERFILES = 
# ---------------------------------------------------------------------------
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
USERDEFINES = _DEBUG
SYSDEFINES = NO_STRICT;_VIS_NOLIB;_ATL_APARTMENT_THREADED;USING_ATLVCL;_ATL_NO_SECURITY;INC_ATL_HEADERS;_BCB_ATL_DEBUG
INCLUDEPATH = "C:\Program Files\Borland\CBuilder6\Projects";$(BCB)\Lib;$(BCB)\include;$(BCB)\include\vcl;$(BCB)\imports;$(BCB)\include\atl
LIBPATH = "C:\Program Files\Borland\CBuilder6\Projects";$(BCB)\lib\obj;$(BCB)\lib;$(BCB)\Projects\lib;$(BCB)\lib\psdk
WARNINGS= -w-par -w-8027 -w-8026
PATHCPP = .;
PATHASM = .;
PATHPAS = .;
PATHRC = .;
PATHOBJ = .;$(LIBPATH)
# ---------------------------------------------------------------------------
CFLAG1 = -WD -Od -H=$(BCB)\lib\vcl60.csm -Hc -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- \
    -c -tWM
IDLCFLAGS = -I"C:\Program Files\Borland\CBuilder6\Projects" -I$(BCB)\Lib \
    -I$(BCB)\include -I$(BCB)\include\vcl -I$(BCB)\imports \
    -I$(BCB)\include\atl -src_suffix cpp -D_DEBUG -boa
PFLAGS = -N2obj -N0obj -$Y+ -$W -$O- -$A8 -v -JPHNE -M
RFLAGS = 
AFLAGS = /mx /w2 /zi
LFLAGS = -lbin -Iobj -D"" -aa -Tpd -x -Gn -Gi -v
# ---------------------------------------------------------------------------
ALLOBJ = c0d32.obj sysinit.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cp32mt.lib
# ---------------------------------------------------------------------------
!ifdef IDEOPTIONS

[Version Info]
IncludeVerInfo=0
AutoIncBuild=0
MajorVer=1
MinorVer=0
Release=0
Build=0
Debug=0
PreRelease=0
Special=0
Private=0
DLL=0

[Version Info Keys]
CompanyName=
FileDescription=
FileVersion=1.0.0.0
InternalName=
LegalCopyright=
LegalTrademarks=
OriginalFilename=
ProductName=
ProductVersion=1.0.0.0
Comments=

[Debugging]
DebugSourceDirs=$(BCB)\source\vcl

!endif





# ---------------------------------------------------------------------------
# MAKE SECTION
# ---------------------------------------------------------------------------
# This section of the project file is not used by the BCB IDE.  It is for
# the benefit of building from the command-line using the MAKE utility.
# ---------------------------------------------------------------------------

.autodepend
# ---------------------------------------------------------------------------
!if "$(USERDEFINES)" != ""
AUSERDEFINES = -d$(USERDEFINES:;= -d)
!else
AUSERDEFINES =
!endif

!if !$d(BCC32)
BCC32 = bcc32
!endif

!if !$d(CPP32)
CPP32 = cpp32
!endif

!if !$d(DCC32)
DCC32 = dcc32
!endif

!if !$d(TASM32)
TASM32 = tasm32
!endif

!if !$d(LINKER)
LINKER = ilink32
!endif

!if !$d(BRCC32)
BRCC32 = brcc32
!endif


# ---------------------------------------------------------------------------
!if $d(PATHCPP)
.PATH.CPP = $(PATHCPP)
.PATH.C   = $(PATHCPP)
!endif

!if $d(PATHPAS)
.PATH.PAS = $(PATHPAS)
!endif

!if $d(PATHASM)
.PATH.ASM = $(PATHASM)
!endif

!if $d(PATHRC)
.PATH.RC  = $(PATHRC)
!endif

!if $d(PATHOBJ)
.PATH.OBJ  = $(PATHOBJ)
!endif
# ---------------------------------------------------------------------------
$(PROJECT): $(OTHERFILES) $(IDLGENFILES) $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) -L$(LIBPATH) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!
# ---------------------------------------------------------------------------
.pas.hpp:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.pas.obj:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.cpp.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.cpp.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.asm.obj:
    $(BCB)\BIN\$(TASM32) $(AFLAGS) -i$(INCLUDEPATH:;= -i) $(AUSERDEFINES) -d$(SYSDEFINES:;= -d) $<, $@

.rc.res:
    $(BCB)\BIN\$(BRCC32) $(RFLAGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -fo$@ $<



# ---------------------------------------------------------------------------




