#include "Archiver.h"

#include <algorithm>
#include <stdexcept>
#include <fstream>

#include "Constants.h"
#include "Reader.h"
#include "Trie.h"
#include "CompareTriePaths.h"

int16_t ReadChar(Reader& reader, const Trie& trie) {
    Trie::Node* cur = trie.root_;
    while (!cur->term_) {
        if (reader.ReadBit() == 0) {
            if (cur->l_ == nullptr) {
                throw std::invalid_argument("Archive is incorrectly formatted");
            }
            cur = cur->l_;
        } else {
            if (cur->r_ == nullptr) {
                throw std::invalid_argument("Archive is incorrectly formatted");
            }
            cur = cur->r_;
        }
    }
    return cur->symbol_;
}

void CalcCnt(Reader& reader, char* file_name, std::unordered_map<int16_t, size_t>& cnt) {
    while (!reader.AllRead()) {
        int16_t c = reader.ReadBits(8);
        ++cnt[c];
    }
    for (char* ptr = file_name; *ptr != '\0'; ++ptr) {
        ++cnt[static_cast<int16_t>(*ptr)];
    }
    ++cnt[FILENAME_END];
    ++cnt[ONE_MORE_FILE];
    ++cnt[ARCHIVE_END];
}

std::vector<TriePath> CalcCanonical(const Trie& trie, const std::unordered_map<int16_t, size_t>& cnt) {
    std::vector<TriePath> paths = trie.FindPaths();
    sort(paths.begin(), paths.end(), CompareTriePaths());
    reverse(paths.begin(), paths.end());

    size_t symbols = cnt.size();
    std::vector<TriePath> canonical(symbols);
    canonical[0] = TriePath(0, paths[0].sz_, paths[0].symbol_);
    for (size_t i = 1; i < canonical.size(); ++i) {
        canonical[i] = TriePath(canonical[i - 1].path_, paths[i].sz_, paths[i].symbol_);
        ++canonical[i].path_;
        canonical[i].path_ <<= (canonical[i].sz_ - canonical[i - 1].sz_);
    }
    for (size_t i = 0; i < canonical.size(); ++i) {
        int64_t flipped_path = 0;
        for (int16_t j = 0; j < canonical[i].sz_; ++j) {
            flipped_path |= (((canonical[i].path_ >> j) & 1) << (canonical[i].sz_ - j - 1));
        }
        canonical[i].path_ = flipped_path;
    }
    return canonical;
}

void WriteSymbol()

void Archiver::Archive(char* file_name, std::istream& input, Writer& writer, bool not_last) {
    Reader reader(input);

    std::unordered_map<int16_t, size_t> cnt;
    CalcCnt(reader, file_name, cnt);

    Trie trie = Trie();
    trie.BuildUsingCnt(cnt);

    std::vector<TriePath> canonical = CalcCanonical(trie, cnt);
    size_t symbols = cnt.size();

    reader.Reset();

    writer.WriteSequence(symbols, 9);
    for (size_t i = 0; i < symbols; ++i) {
        writer.WriteSequence(canonical[i].symbol_, 9);
    }
    int16_t max_symbol_code_size = canonical[symbols - 1].sz_;
    std::vector<int16_t> szcnt(max_symbol_code_size + 1, 0);
    for (size_t i = 0; i < symbols; ++i) {
        ++szcnt[canonical[i].sz_];
    }
    for (size_t i = 1; i <= max_symbol_code_size; ++i) {
        writer.WriteSequence(szcnt[i], 9);
    }

    std::unordered_map<int16_t, size_t> symbol_ind;
    for (size_t i = 0; i < symbols; ++i) {
        symbol_ind[canonical[i].symbol_] = i;
    }

    for (char* ptr = file_name; *ptr != '\0'; ++ptr) {
        size_t ind = symbol_ind[*ptr];
        writer.WriteSequence(canonical[ind].path_, canonical[ind].sz_);
    }

    size_t ind = symbol_ind[FILENAME_END];
    writer.WriteSequence(canonical[ind].path_, canonical[ind].sz_);

    while (!reader.AllRead()) {
        ind = symbol_ind[reader.ReadBits(8)];
        writer.WriteSequence(canonical[ind].path_, canonical[ind].sz_);
    }

    if (not_last) {
        ind = symbol_ind[ONE_MORE_FILE];
        writer.WriteSequence(canonical[ind].path_, canonical[ind].sz_);
    } else {
        ind = symbol_ind[ARCHIVE_END];
        writer.WriteSequence(canonical[ind].path_, canonical[ind].sz_);
        writer.Done();
    }
}

void Archiver::Unarchive(std::istream& input) {
    Reader reader(input);

    while (!reader.AllRead()) {
        int16_t symbols = reader.ReadBits(9);
        if (symbols == 0) {
            throw std::invalid_argument("Archive is incorrectly formatted");
        }

        std::vector<TriePath> canonical(symbols);
        for (size_t i = 0; i < static_cast<size_t>(symbols); ++i) {
            canonical[i].symbol_ = reader.ReadBits(9);
        }

        int16_t sum = 0;
        std::vector<int16_t> szcnt(1, 0);
        while (sum < symbols) {
            int16_t nw = reader.ReadBits(9);
            if (sum + nw > symbols) {
                throw std::invalid_argument("Archive is incorrectly formatted");
            }
            sum += nw;
            szcnt.push_back(nw);
        }

        size_t ptr = 0;
        for (size_t i = 1; i < szcnt.size(); ++i) {
            if (szcnt[i] != 0) {
                canonical[0].path_ = 0;
                canonical[0].sz_ = i;
                ptr = i;
                --szcnt[ptr];
                break;
            }
            if (i + 1 == szcnt.size()) {
                throw std::invalid_argument("Archive is incorrectly formatted");
            }
        }
        for (size_t i = 1; i < symbols; ++i) {
            while (szcnt[ptr] == 0) {
                ++ptr;
            }
            --szcnt[ptr];
            canonical[i].sz_ = ptr;
            canonical[i].path_ = canonical[i - 1].path_ + 1;
            canonical[i].path_ <<= (canonical[i].sz_ - canonical[i - 1].sz_);
        }
        for (size_t i = 0; i < symbols; ++i) {
            int64_t flipped_path = 0;
            for (int16_t j = 0; j < canonical[i].sz_; ++j) {
                flipped_path |= (((canonical[i].path_ >> j) & 1) << (canonical[i].sz_ - j - 1));
            }
            canonical[i].path_ = flipped_path;
        }

        Trie trie;
        for (size_t i = 0; i < symbols; ++i) {
            trie.AddPath(canonical[i]);
        }

        std::string file_name;

        int16_t c = ReadChar(reader, trie);
        while (c != FILENAME_END) {
            if (c > 255) {
                throw std::invalid_argument("Archive is incorrectly formatted");
            }
            file_name.push_back(char(c));
            c = ReadChar(reader, trie);
        }

        std::ofstream file_output;
        file_output.open(file_name);
        if (!file_output.is_open()) {
            throw std::invalid_argument("Couldn't open one of the files");
        }
        Writer writer(file_output);

        c = ReadChar(reader, trie);
        while (c != ONE_MORE_FILE && c != ARCHIVE_END) {
            if (c > 255) {
                throw std::invalid_argument("Archive is incorrectly formatted");
            }
            writer.WriteSequence(c, 8);
            c = ReadChar(reader, trie);
        }

        if (c == ARCHIVE_END) {
            writer.Done();
            break;
        }

        file_output.close();
    }
}

void Archiver::Parse(int argc, char** argv) {
    if (argc <= 2) {
        throw std::invalid_argument("Not enough arguments");
    }
    if (std::strcmp(argv[2], "-h") == 0) {
        if (argc > 3) {
            throw std::invalid_argument("Use \"archiver -h\" for help");
        }
        std::cout << "This is my archiver\n"
                     "If you want to archive files file1, file2 and so on into an archive called archive_name, use\n"
                     "\"archiver -c archive_name file1 [file2 ...]\n"
                     "If you want to unarchive file archive_name, use \"archiver -d archive_name\"\n";
        return;
    }
    if (std::strcmp(argv[2], "-d") == 0) {
        if (argc != 4) {
            throw std::invalid_argument("Wrong number of arguments");
        }
        std::ifstream input;
        input.open(argv[3]);
        if (!input.is_open()) {
            throw std::runtime_error("Couldn't open the archive file");
        }
        Unarchive(input);
        input.close();
        return;
    }
    if (std::strcmp(argv[2], "-c") == 0) {
        if (argc < 5) {
            throw std::invalid_argument("Not enough arguments");
        }

        std::ofstream output;
        output.open(argv[3], std::ofstream::out | std::ofstream::trunc);
        if (!output.is_open()) {
            throw std::runtime_error("Couldn't open the archive file");
        }
        output.close();
        output.open(argv[3], std::ios::app);
        if (!output.is_open()) {
            throw std::runtime_error("Couldn't open the archive file");
        }

        Writer writer(output);

        for (size_t i = 4; i < argc; ++i) {
            std::ifstream input;
            input.open(argv[i]);
            if (!input.is_open()) {
                throw std::runtime_error("Couldn't open one of the files");
            }
            Archive(argv[i], input, writer, (i + 1 != argc));
            input.close();
        }
        output.close();
        return;
    }
    throw std::invalid_argument("Incorrect option");
}

void Archiver::Archive(char *file_name, std::istream &input, std::ostream &output) {
    Writer writer(output);
    Archive(file_name, input, writer, 0);
}

