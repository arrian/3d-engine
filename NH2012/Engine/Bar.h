#pragma once



class Bar
{
public:
  Bar(void);
  Bar(float maxValue);
  Bar(float maxValue, float currentValue);
  ~Bar(void);

  /*! Returns true if the bar is empty.*/
  bool isEmpty();

  /*! Returns true if the bar is full.*/
  bool isFull();

  /*! Gets the fraction of the bar that is full.*/
  float getFraction();

  float getMax();
  float getCurrent();

  void setCurrent(float current);
  void setMax(float max);

  void setFull();
  void setEmpty();

  void add(float amount);
  void remove(float amount);

private:
  /*! Maximum value of the bar.*/
  float max;

  /*! Current value of the bar.*/
  float current;
};

