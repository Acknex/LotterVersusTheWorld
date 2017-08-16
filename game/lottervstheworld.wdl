PATH "level";
PATH "models";
PATH "sounds";
PATH "shaders";
PATH "src"; //fuck you, WED

WINDOW WINSTART
{
  TITLE "Lotter vs. the World";
  SIZE 740,740;
  MODE IMAGE;	
  BG_COLOR RGB(0, 0, 0);
  BG_PATTERN <start256.pcx>,OPAQUE;
  PROGRESS RGB(255, 127, 255), 30, 100, 710, 640, 30;
}