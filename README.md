# Projekat iz Osnova softverskog inženjerstva 

Ovo je repozitorijum namijenjen za izvorni kod studentskog projekta iz predmeta **Osnovi softverskog inženjerstva** na Elektrotehničkom fakultetu Univerziteta u Banjoj Luci.

---

## Struktura projekta

| Folder | Opis |
| :--- | :--- |
| `src/` | Izvršni kod  |
| `include/` | Header fajlovi |
| `docs/` | Dokumentacija vezana za projekat |
| `tests/` | Unit testovi  |
| `data/` | Podaci neophodni za rad aplikacije |

---

## Instrukcije za kompajliranje

Najlakše je pozvati `.\compile.bat` skriptu iz korijena projekta. 
Ukoliko želite ručno pokretati build, testiranje i instalaciju, koristite komande ispod zavisno od vašeg operativnog sistema i kompajlera.

### Windows (MSVC)
*Za korisnike Visual Studio kompajlera:*
```bash
cmake --preset=msvc-release && cmake --build --preset=msvc-release && cd build && ctest -C Release --output-on-failure && cd .. && cmake --install build --config Release --prefix "build" && cd build && for /D %d in (*) do @if /I not "%d"=="bin" rmdir /S /Q "%d" && del /Q *.* 2>nul && cd .. && echo "MSVC Build, Test i Instalacija GOTOVI!"
```

### Windows (MinGW)
*Za korisnike Za korisnike GCC/MinGW okruženja:*
```bash
cmake --preset=mingw-release && cmake --build --preset=mingw-release && cd build && ctest --output-on-failure && cd .. && cmake --install build --prefix "build" && cd build && for /D %d in (*) do @if /I not "%d"=="bin" rmdir /S /Q "%d" && del /Q *.* 2>nul && cd .. && echo "MinGW Build, Test i Instalacija GOTOVI!"
```

### Linux 
*Standardni build proces za Linux sisteme:*
```bash
cmake --preset=linux-release && cmake --build --preset=linux-release && cd build && ctest --output-on-failure && cd .. && cmake --install build --prefix "build" && find build -maxdepth 1 ! -name bin ! -name build -exec rm -rf {} + && echo "Linux Build, Test i Instalacija GOTOVA!"
```

###NAPONENA: 
*Sve komande su konfigurisane tako da nakon uspješne instalacije brišu sve privremene build fajlove i izvršne fajlove testova, ostavljajući samo izvršne fajlove u `build/bin` folderu*