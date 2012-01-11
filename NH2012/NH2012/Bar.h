#pragma once
class Bar
{
public:
  Bar();
  Bar(int maxValue);
  ~Bar(void);

  int getCurrent();
  int getMax();

  void increaseMax(int amount);
  void increaseCurrent(int amount);
private:
  int max;
  int current;
};

