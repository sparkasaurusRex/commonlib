#pragma once

namespace Game
{
  class Condition
  {
  protected:
    uint32_t hash_id;
  public:
    Condition() { hash_id = 0; }
    ~Condition() {}

    void set_name(char *n) { hash_id = Math::hash_value_from_string(n); }
    uint32_t get_hash_id() const { return hash_id; }

    virtual bool condition_met() const = 0;
  };

  class ConditionBool : public Condition
  {
  protected:
    bool *var;
  public:
    ConditionBool() : Condition() { var = NULL; }
    ~ConditionBool() {}

    virtual bool condition_met() const { return *var; }

    void set_var_ptr(bool *b) { var = b; }
  };

  class ConditionFloat : public Condition
  {
  protected:
    float *var;
    float target;
    float error;
  public:
    ConditionFloat() : Condition() { target = 0.5f; error = 0.0001f; }
    ~ConditionFloat() {}

    void set_goal(float goal, float err = 0.0001f) { target = goal; error = err; }
    void set_var_ptr(float *f) { var = f; }

    virtual bool condition_met() { return (fabs(*var - target) < error); }
  };
};