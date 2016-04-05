#include "../Extensions/Crawler.h"
#include "../ROOT/ROOT.h"

int main(){

  Crawler c("ntupla.root");
  c.SetBaseDirectory("resources/Extensions/test/");
  c.CreateDictionary("resources/Extensions/test/run160955/database_new.txt", 8);
  c.PrintDictionary();
//Just for old runs:
//  c.SetRegularExpression("RawID,Name,Channel,IsDisconnected,IsDead,IsMasked,Rate,RateHz");

//For new runs:
  c.SetRegularExpression("RawID,Channel,IsDisconnected,IsDead,IsMasked,Rate");
  c.ScanDirectory();
  c.Crawl("database_full.txt", 8);
  c.WriteRootFile();

  return 0;
}
