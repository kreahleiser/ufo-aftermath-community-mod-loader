#ifndef UFO_GAMEPATH_H
#define UFO_GAMEPATH_H

int has_ufo_exe(const char *dir);
int is_ufo_exe_filename(const char *path);
int game_folder_from_ufo_exe(const char *exePath, char *out, int n);
int find_steam_ufo(char *out, int n);
int find_game(char *out, int n);

#endif
