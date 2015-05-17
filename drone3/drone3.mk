##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=drone3
ConfigurationName      :=Debug
WorkspacePath          := "/home/frank/.codelite/drone"
ProjectPath            := "/home/frank/project/drone/test/drone3"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=frank
Date                   :=16.05.2015
CodeLitePath           :="/home/frank/.codelite"
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="drone3.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/home/frank/project/drone/sdk3/ARSDKBuildUtils/Targets/Unix/Install/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)arsal $(LibrarySwitch)arcommands $(LibrarySwitch)arnetwork $(LibrarySwitch)arnetworkal $(LibrarySwitch)ardiscovery $(LibrarySwitch)arstream $(LibrarySwitch)ncurses $(LibrarySwitch)avcodec $(LibrarySwitch)avformat $(LibrarySwitch)swscale $(LibrarySwitch)avutil 
ArLibs                 :=  "arsal" "arcommands" "arnetwork" "arnetworkal" "ardiscovery" "arstream" "ncurses" "avcodec" "avformat" "swscale" "avutil" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/home/frank/project/drone/sdk3/ARSDKBuildUtils/Targets/Unix/Install/lib64 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/BebopDroneDecodeStream$(ObjectSuffix) $(IntermediateDirectory)/DecoderManager$(ObjectSuffix) $(IntermediateDirectory)/ihm$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.c $(IntermediateDirectory)/main$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/frank/project/drone/test/drone3/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "main.c"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "main.c"

$(IntermediateDirectory)/BebopDroneDecodeStream$(ObjectSuffix): BebopDroneDecodeStream.c $(IntermediateDirectory)/BebopDroneDecodeStream$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/frank/project/drone/test/drone3/BebopDroneDecodeStream.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BebopDroneDecodeStream$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BebopDroneDecodeStream$(DependSuffix): BebopDroneDecodeStream.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BebopDroneDecodeStream$(ObjectSuffix) -MF$(IntermediateDirectory)/BebopDroneDecodeStream$(DependSuffix) -MM "BebopDroneDecodeStream.c"

$(IntermediateDirectory)/BebopDroneDecodeStream$(PreprocessSuffix): BebopDroneDecodeStream.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BebopDroneDecodeStream$(PreprocessSuffix) "BebopDroneDecodeStream.c"

$(IntermediateDirectory)/DecoderManager$(ObjectSuffix): DecoderManager.c $(IntermediateDirectory)/DecoderManager$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/frank/project/drone/test/drone3/DecoderManager.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DecoderManager$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DecoderManager$(DependSuffix): DecoderManager.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DecoderManager$(ObjectSuffix) -MF$(IntermediateDirectory)/DecoderManager$(DependSuffix) -MM "DecoderManager.c"

$(IntermediateDirectory)/DecoderManager$(PreprocessSuffix): DecoderManager.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DecoderManager$(PreprocessSuffix) "DecoderManager.c"

$(IntermediateDirectory)/ihm$(ObjectSuffix): ihm.c $(IntermediateDirectory)/ihm$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/frank/project/drone/test/drone3/ihm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ihm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ihm$(DependSuffix): ihm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ihm$(ObjectSuffix) -MF$(IntermediateDirectory)/ihm$(DependSuffix) -MM "ihm.c"

$(IntermediateDirectory)/ihm$(PreprocessSuffix): ihm.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ihm$(PreprocessSuffix) "ihm.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/BebopDroneDecodeStream$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/BebopDroneDecodeStream$(DependSuffix)
	$(RM) $(IntermediateDirectory)/BebopDroneDecodeStream$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DecoderManager$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DecoderManager$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DecoderManager$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ihm$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ihm$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ihm$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../../../../.codelite/drone/.build-debug/drone3"


