# Cybrinth created by James Dearing, copyright 2012-2017 and licensed under the GNU AGPL (see the file COPYING)

#Note: Consider making a new makefile based on examples from http://nuclear.mutantstargoat.com/articles/make/

export DEB_BUILD_HARDENING:= 1 #If you have hardening wrapper scripts installed (Ubuntu/Debian package "hardening-wrapper"), this should make them harden the program

IMAGES_RESULT != [ `find ./Images -type f -printf "%T@\n" | sort | tail -n 1 | xargs printf "%.f"` -gt `find ./compiled-images -type f -printf "%T@\n" | sort | tail -n 1 | xargs printf "%.f"` ] && echo 1 || echo 0

AM_CPPFLAGS = -Wall -Wextra -Wno-write-strings -Wno-pedantic $(IRRLICHT_CFLAGS) $(BOOSTFILESYSTEM_CFLAGS) $(BOOSTSYSTEM_CFLAGS) $(DEPS_CFLAGS) $(SDLDEPS_CFLAGS)
AM_LDFLAGS = -lstdc++ -lm -pthread
bin_PROGRAMS = cybrinth

BUILT_SOURCES = compiled-images

cybrinth_SOURCES = src/SettingsManager.h src/SettingsManager.cpp src/SettingsScreen.h src/SettingsScreen.cpp src/CustomException.h src/CustomException.cpp src/Integers.h src/XPMImageLoader.h src/XPMImageLoader.cpp src/AI.h src/AI.cpp src/Collectable.h src/Collectable.cpp src/colors.h src/FontManager.h src/FontManager.cpp src/MainGame.h src/MainGame.cpp src/Goal.h src/Goal.cpp src/GUIFreetypeFont.h src/GUIFreetypeFont.cpp src/ControlMapping.h src/ControlMapping.cpp src/main.cpp src/MazeCell.h src/MazeCell.cpp src/MazeManager.h src/MazeManager.cpp src/MenuOption.h src/MenuOption.cpp src/NetworkManager.h src/NetworkManager.cpp src/Object.h src/Object.cpp src/Player.h src/Player.cpp src/PlayerStart.h src/PlayerStart.cpp src/StringConverter.h src/StringConverter.cpp src/SpellChecker.h src/SpellChecker.cpp src/ImageModifier.h src/ImageModifier.cpp src/SystemSpecificsManager.h src/SystemSpecificsManager.cpp src/PreprocessorCommands.h src/MenuManager.h  src/MenuManager.cpp src/FileSelectorDialog.h src/FileSelectorDialog.cpp src/RakNet/AutopatcherPatchContext.h src/RakNet/AutopatcherRepositoryInterface.h src/RakNet/Base64Encoder.cpp src/RakNet/Base64Encoder.h src/RakNet/BitStream.cpp src/RakNet/BitStream.h src/RakNet/CCRakNetSlidingWindow.cpp src/RakNet/CCRakNetSlidingWindow.h src/RakNet/CCRakNetUDT.cpp src/RakNet/CCRakNetUDT.h src/RakNet/CheckSum.cpp src/RakNet/CheckSum.h src/RakNet/CloudClient.cpp src/RakNet/CloudClient.h src/RakNet/CloudCommon.cpp src/RakNet/CloudCommon.h src/RakNet/CloudServer.cpp src/RakNet/CloudServer.h src/RakNet/CMakeLists.txt src/RakNet/CommandParserInterface.cpp src/RakNet/CommandParserInterface.h src/RakNet/ConnectionGraph2.cpp src/RakNet/ConnectionGraph2.h src/RakNet/ConsoleServer.cpp src/RakNet/ConsoleServer.h src/RakNet/DataCompressor.cpp src/RakNet/DataCompressor.h src/RakNet/DirectoryDeltaTransfer.cpp src/RakNet/DirectoryDeltaTransfer.h src/RakNet/DR_SHA1.cpp src/RakNet/DR_SHA1.h src/RakNet/DS_BinarySearchTree.h src/RakNet/DS_BPlusTree.h src/RakNet/DS_BytePool.cpp src/RakNet/DS_BytePool.h src/RakNet/DS_ByteQueue.cpp src/RakNet/DS_ByteQueue.h src/RakNet/DS_Hash.h src/RakNet/DS_Heap.h src/RakNet/DS_HuffmanEncodingTree.cpp src/RakNet/DS_HuffmanEncodingTreeFactory.h src/RakNet/DS_HuffmanEncodingTree.h src/RakNet/DS_HuffmanEncodingTreeNode.h src/RakNet/DS_LinkedList.h src/RakNet/DS_List.h src/RakNet/DS_Map.h src/RakNet/DS_MemoryPool.h src/RakNet/DS_Multilist.h src/RakNet/DS_OrderedChannelHeap.h src/RakNet/DS_OrderedList.h src/RakNet/DS_Queue.h src/RakNet/DS_QueueLinkedList.h src/RakNet/DS_RangeList.h src/RakNet/DS_Table.cpp src/RakNet/DS_Table.h src/RakNet/DS_ThreadsafeAllocatingQueue.h src/RakNet/DS_Tree.h src/RakNet/DS_WeightedGraph.h src/RakNet/DynDNS.cpp src/RakNet/DynDNS.h src/RakNet/EmailSender.cpp src/RakNet/EmailSender.h src/RakNet/EmptyHeader.h src/RakNet/EpochTimeToString.cpp src/RakNet/EpochTimeToString.h src/RakNet/Export.h src/RakNet/FileList.cpp src/RakNet/FileList.h src/RakNet/FileListNodeContext.h src/RakNet/FileListTransferCBInterface.h src/RakNet/FileListTransfer.cpp src/RakNet/FileListTransfer.h src/RakNet/FileOperations.cpp src/RakNet/FileOperations.h src/RakNet/_FindFirst.cpp src/RakNet/_FindFirst.h src/RakNet/FormatString.cpp src/RakNet/FormatString.h src/RakNet/FullyConnectedMesh2.cpp src/RakNet/FullyConnectedMesh2.h src/RakNet/Getche.cpp src/RakNet/Getche.h src/RakNet/Gets.cpp src/RakNet/Gets.h src/RakNet/GetTime.cpp src/RakNet/GetTime.h src/RakNet/gettimeofday.cpp src/RakNet/gettimeofday.h src/RakNet/GridSectorizer.cpp src/RakNet/GridSectorizer.h src/RakNet/HTTPConnection2.cpp src/RakNet/HTTPConnection2.h src/RakNet/HTTPConnection.cpp src/RakNet/HTTPConnection.h src/RakNet/IncrementalReadInterface.cpp src/RakNet/IncrementalReadInterface.h src/RakNet/InternalPacket.h src/RakNet/Itoa.cpp src/RakNet/Itoa.h src/RakNet/Kbhit.h src/RakNet/LinuxStrings.cpp src/RakNet/LinuxStrings.h src/RakNet/LocklessTypes.cpp src/RakNet/LocklessTypes.h src/RakNet/LogCommandParser.cpp src/RakNet/LogCommandParser.h src/RakNet/MessageFilter.cpp src/RakNet/MessageFilter.h src/RakNet/MessageIdentifiers.h src/RakNet/MTUSize.h src/RakNet/NativeFeatureIncludes.h src/RakNet/NativeFeatureIncludesOverrides.h src/RakNet/NativeTypes.h src/RakNet/NatPunchthroughClient.cpp src/RakNet/NatPunchthroughClient.h src/RakNet/NatPunchthroughServer.cpp src/RakNet/NatPunchthroughServer.h src/RakNet/NatTypeDetectionClient.cpp src/RakNet/NatTypeDetectionClient.h src/RakNet/NatTypeDetectionCommon.cpp src/RakNet/NatTypeDetectionCommon.h src/RakNet/NatTypeDetectionServer.cpp src/RakNet/NatTypeDetectionServer.h src/RakNet/NetworkIDManager.cpp src/RakNet/NetworkIDManager.h src/RakNet/NetworkIDObject.cpp src/RakNet/NetworkIDObject.h src/RakNet/PacketConsoleLogger.cpp src/RakNet/PacketConsoleLogger.h src/RakNet/PacketFileLogger.cpp src/RakNet/PacketFileLogger.h src/RakNet/PacketizedTCP.cpp src/RakNet/PacketizedTCP.h src/RakNet/PacketLogger.cpp src/RakNet/PacketLogger.h src/RakNet/PacketOutputWindowLogger.cpp src/RakNet/PacketOutputWindowLogger.h src/RakNet/PacketPool.h src/RakNet/PacketPriority.h src/RakNet/PluginInterface2.cpp src/RakNet/PluginInterface2.h src/RakNet/PS3Includes.h src/RakNet/PS4Includes.cpp src/RakNet/PS4Includes.h src/RakNet/Rackspace.cpp src/RakNet/Rackspace.h src/RakNet/RakAlloca.h src/RakNet/RakAssert.h src/RakNet/RakMemoryOverride.cpp src/RakNet/RakMemoryOverride.h src/RakNet/RakNetCommandParser.cpp src/RakNet/RakNetCommandParser.h src/RakNet/RakNetDefines.h src/RakNet/RakNetDefinesOverrides.h src/RakNet/RakNetSmartPtr.h src/RakNet/RakNetSocket2_360_720.cpp src/RakNet/RakNetSocket2_Berkley.cpp src/RakNet/RakNetSocket2_Berkley_NativeClient.cpp src/RakNet/RakNetSocket2.cpp src/RakNet/RakNetSocket2.h src/RakNet/RakNetSocket2_NativeClient.cpp src/RakNet/RakNetSocket2_PS3_PS4.cpp src/RakNet/RakNetSocket2_PS4.cpp src/RakNet/RakNetSocket2_Vita.cpp src/RakNet/RakNetSocket2_Windows_Linux_360.cpp src/RakNet/RakNetSocket2_Windows_Linux.cpp src/RakNet/RakNetSocket2_WindowsStore8.cpp src/RakNet/RakNetSocket.cpp src/RakNet/RakNetSocket.h src/RakNet/RakNetStatistics.cpp src/RakNet/RakNetStatistics.h src/RakNet/RakNetTime.h src/RakNet/RakNetTransport2.cpp src/RakNet/RakNetTransport2.h src/RakNet/RakNetTypes.cpp src/RakNet/RakNetTypes.h src/RakNet/RakNet_vc8.vcproj src/RakNet/RakNet_vc9.vcproj src/RakNet/RakNet.vcproj src/RakNet/RakNetVersion.h src/RakNet/RakPeer.cpp src/RakNet/RakPeer.h src/RakNet/RakPeerInterface.h src/RakNet/RakSleep.cpp src/RakNet/RakSleep.h src/RakNet/RakString.cpp src/RakNet/RakString.h src/RakNet/RakThread.cpp src/RakNet/RakThread.h src/RakNet/RakWString.cpp src/RakNet/RakWString.h src/RakNet/Rand.cpp src/RakNet/Rand.h src/RakNet/RandSync.cpp src/RakNet/RandSync.h src/RakNet/ReadyEvent.cpp src/RakNet/ReadyEvent.h src/RakNet/RefCountedObj.h src/RakNet/RelayPlugin.cpp src/RakNet/RelayPlugin.h src/RakNet/ReliabilityLayer.cpp src/RakNet/ReliabilityLayer.h src/RakNet/ReplicaEnums.h src/RakNet/ReplicaManager3.cpp src/RakNet/ReplicaManager3.h src/RakNet/Router2.cpp src/RakNet/Router2.h src/RakNet/RPC4Plugin.cpp src/RakNet/RPC4Plugin.h src/RakNet/SecureHandshake.cpp src/RakNet/SecureHandshake.h src/RakNet/SendToThread.cpp src/RakNet/SendToThread.h src/RakNet/SignaledEvent.cpp src/RakNet/SignaledEvent.h src/RakNet/SimpleMutex.cpp src/RakNet/SimpleMutex.h src/RakNet/SimpleTCPServer.h src/RakNet/SingleProducerConsumer.h src/RakNet/SocketDefines.h src/RakNet/SocketIncludes.h src/RakNet/SocketLayer.cpp src/RakNet/SocketLayer.h src/RakNet/StatisticsHistory.cpp src/RakNet/StatisticsHistory.h src/RakNet/StringCompressor.cpp src/RakNet/StringCompressor.h src/RakNet/StringTable.cpp src/RakNet/StringTable.h src/RakNet/SuperFastHash.cpp src/RakNet/SuperFastHash.h src/RakNet/TableSerializer.cpp src/RakNet/TableSerializer.h src/RakNet/TCPInterface.cpp src/RakNet/TCPInterface.h src/RakNet/TeamBalancer.cpp src/RakNet/TeamBalancer.h src/RakNet/TeamManager.cpp src/RakNet/TeamManager.h src/RakNet/TelnetTransport.cpp src/RakNet/TelnetTransport.h src/RakNet/ThreadPool.h src/RakNet/ThreadsafePacketLogger.cpp src/RakNet/ThreadsafePacketLogger.h src/RakNet/TransportInterface.h src/RakNet/TwoWayAuthentication.cpp src/RakNet/TwoWayAuthentication.h src/RakNet/UDPForwarder.cpp src/RakNet/UDPForwarder.h src/RakNet/UDPProxyClient.cpp src/RakNet/UDPProxyClient.h src/RakNet/UDPProxyCommon.h src/RakNet/UDPProxyCoordinator.cpp src/RakNet/UDPProxyCoordinator.h src/RakNet/UDPProxyServer.cpp src/RakNet/UDPProxyServer.h src/RakNet/VariableDeltaSerializer.cpp src/RakNet/VariableDeltaSerializer.h src/RakNet/VariableListDeltaTracker.cpp src/RakNet/VariableListDeltaTracker.h src/RakNet/VariadicSQLParser.cpp src/RakNet/VariadicSQLParser.h src/RakNet/VitaIncludes.cpp src/RakNet/VitaIncludes.h src/RakNet/WindowsIncludes.h src/RakNet/WSAStartupSingleton.cpp src/RakNet/WSAStartupSingleton.h src/RakNet/XBox360Includes.h

# cybrinth_SOURCES = $(wildcard src/*.h src/*.cpp)
# cybrinth_SOURCES += compiled-images/key.xpm compiled-images/acid.xpm compiled-images/goal.xpm compiled-images/start.xpm
# cybrinth_SOURCES += $(wildcard compiled-images/*.cpp)
cybrinth_LDADD = $(IRRLICHT_LIBS) $(BOOSTFILESYSTEM_LIBS) $(BOOSTFILESYSTEM_LIBS_TWO) $(BOOSTSYSTEM_LIBS) $(BOOSTSYSTEM_LIBS_TWO) $(DEPS_LIBS) $(SDLDEPS_LIBS)

clean-local: remove-compiled-images

distclean-local: remove-compiled-images

compiled-images:
	if [ "$(IMAGES_RESULT)" -eq "1" ];\
	then rm -rf ./compiled-images ;\
	mkdir ./compiled-images ;\
	mkdir ./compiled-images/items ;\
	mkdir "./compiled-images/menu icons" ;\
	mkdir ./compiled-images/players ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/items/key.xcf" "./compiled-images/items/key.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/items/acid.xcf" "./compiled-images/items/acid.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/goal.xcf" "./compiled-images/goal.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/start.xcf" "./compiled-images/start.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/new_maze.svg" "./compiled-images/menu icons/new_maze.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/restart_maze.svg" "./compiled-images/menu icons/restart_maze.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/load_maze.svg" "./compiled-images/menu icons/load_maze.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/save_maze.svg" "./compiled-images/menu icons/save_maze.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/settings.svg" "./compiled-images/menu icons/settings.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/exit_game.svg" "./compiled-images/menu icons/exit_game.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/back_to_game.svg" "./compiled-images/menu icons/back_to_game.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/freedom.svg" "./compiled-images/menu icons/freedom.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/cancel.svg" "./compiled-images/menu icons/cancel.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/ok.svg" "./compiled-images/menu icons/ok.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/undo_changes.svg" "./compiled-images/menu icons/undo_changes.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/reset_to_defaults.svg" "./compiled-images/menu icons/reset_to_defaults.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/menu icons/join_server.svg" "./compiled-images/menu icons/join_server.xpm" ;\
	convert -background 'rgba(0,0,0,0)' -flatten "./Images/players/poker_chip.xcf" "./compiled-images/players/poker_chip.xpm" ;\
	fi

remove-compiled-images:
	rm -rf compiled-images

install-data-local:
	cat gimp-save-as-png.py | ${XVFB_RUN_CMD} gimp --no-splash --no-interface --no-data --console-messages --batch-interpreter=python-fu-eval --batch -;\
	mkdir --parents --mode=7777 $(DESTDIR)@datadir@/Cybrinth
	cp --recursive ./Music $(DESTDIR)@datadir@/Cybrinth/Music
	cp --recursive ./Images $(DESTDIR)@datadir@/Cybrinth/Images
	cp --recursive ./Fonts $(DESTDIR)@datadir@/Cybrinth/Fonts
	cp ./protips.txt $(DESTDIR)@datadir@/Cybrinth/protips.txt
	cp ./exitConfirmations.txt $(DESTDIR)@datadir@/Cybrinth/exitConfirmations.txt
	cp ./credits.txt $(DESTDIR)@datadir@/Cybrinth/credits.txt
	mkdir --parents --mode=7777 $(DESTDIR)@sysconfdir@/Cybrinth
	cp ./prefs.cfg $(DESTDIR)@sysconfdir@/Cybrinth/prefs.cfg
	cp ./controls.cfg $(DESTDIR)@sysconfdir@/Cybrinth/controls.cfg

uninstall-local:
	rm -rf @datadir@/Cybrinth/

.PHONY: compiled-images remove-compiled-images
