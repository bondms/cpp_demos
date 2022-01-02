// Copyright 2021 Mark Bond

#pragma once

class CountdownClientInterface {
public:
  virtual void OnCountdownTick(int value);
};
