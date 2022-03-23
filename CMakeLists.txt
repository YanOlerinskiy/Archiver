project(archiver)

add_executable(archiver
    main.cpp
        Reader.cpp Reader.h Writer.cpp Writer.h Trie.cpp Trie.h TriePath.cpp TriePath.h CompareTriePaths.cpp
        CompareTriePaths.h Constants.h Archiver.cpp Archiver.h)
