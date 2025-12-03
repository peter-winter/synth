#pragma once

template <typename Generator, typename MulGen, typename AddGen>
class mul_add
{
public:
    explicit mul_add(Generator&& gen, MulGen&& mul_gen, AddGen&& add_gen)
        : gen_(std::forward<Generator>(gen)), mul_gen_(std::forward<MulGen>(mul_gen)), add_gen_(std::forward<AddGen>(add_gen))
    {
    }

    float operator()()
    {
        return gen_() * mul_gen_() + add_gen_();
    }

private:
    Generator gen_;
    MulGen    mul_gen_;
    AddGen    add_gen_;
};
