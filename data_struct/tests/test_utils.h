#pragma once
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <functional>

// checks
namespace test_util_details
{
    struct TestFailure
    {
        std::string msg;
        const char* file;
        int line;
    };

    inline void fail(
        bool cond,
        const std::string& msg,
        const char* file,
        int line
    )
    {
        if (!cond)
        {
            throw TestFailure{
                msg,
                file,
                line
            };
        }
    }

    inline void check_impl(
        bool cond,
        const char* cond_str,
        const char* file,
        int line
    )
    {
        fail(
            cond,
            std::string("CHECK(") + cond_str + ")",
            file,
            line
        );
    }

    template <typename L_type, typename R_type>
    void check_equal_impl(
        const L_type& a, 
        const R_type& b, 
        const char* a_str, 
        const char* b_str, 
        const char* file, 
        int line
    )
    {
        fail(
            a == b,
            std::string("CHECK_EQUAL(") + a_str + " == " + b_str +")",
            file,
            line
        );
    }

    template <typename Container>
    void check_container_invariants_impl(
        const Container& c,
        const char* c_str,
        const char* file,
        int line
    )
    {
        auto fail_check = [&](
            bool cond, 
            const std::string& cond_str
        )
        {
            fail(
                cond,
                std::string("CHECK_CONTAINER_INVARIANTS(") + c_str + ": " + cond_str + ")",
                file,
                line
            );
        };

        fail_check(
            c.length() <= c.capacity(),
            "c.length() <= c.capacity()"
        );
        fail_check(
            c.empty() == (c.length() == 0),
            "empty() consist with length()"
        );
        if (c.length() > 0)
        {
            fail_check(
                c.data() != nullptr,
                "data() is not nullptr when length() > 0"
            );
        }
        fail_check(
            static_cast<std::size_t>(c.end() - c.begin()) == c.length(),
            "iterator distance == length()"
        );
    }

    template<typename Exception_type, typename Func>
    void check_throws_impl(
        Func&& func,
        const char* func_str,
        const char* exception_type_str,
        const char* file,
        int line
    )
    {
        bool thrown = false;
        try
        {
            func();
        }
        catch (const Exception_type&)
        {
            thrown = true;
        }
        catch(...)
        {
        }

        fail(
            thrown,
            std::string("CHECK_THROWS(") + func_str + ", " + exception_type_str + ")",
            file,
            line
        );
    }
}

#define CHECK(cond)    \
    test_util_details::check_impl((cond), #cond, __FILE__, __LINE__)

#define CHECK_EQUAL(a, b)   \
    test_util_details::check_equal_impl((a), (b), #a, #b, __FILE__, __LINE__)

#define CHECK_CONTAINER_INVARIANTS(container)   \
    test_util_details::check_container_invariants_impl((container), #container, __FILE__, __LINE__)

#define CHECK_THROWS(func, exception_type)   \
    test_util_details::check_throws_impl<exception_type>(   \
        [&]() { (void)(func); }, #func, #exception_type, __FILE__, __LINE__)

// Tracker: lifetime examine
struct Tracker
{
    // counter, recording the times 
    // of construction, destruction, copy and move
    static int construct_count_;
    static int destruct_count_;
    static int copy_count_;
    static int move_count_;

    int value_;

    // constructor, copy controller
    Tracker()
        : value_(0)
    {
        ++construct_count_;
    }
    explicit Tracker(int value)
        : value_(value)
    {
        ++construct_count_;
    }

    Tracker(const Tracker& init)
        : value_(init.value_)
    {
        ++construct_count_;
        ++copy_count_;
    }
    Tracker(Tracker&& init) noexcept
        : value_(std::move(init.value_))
    {
        ++construct_count_;
        ++move_count_;
        init.value_ = -1;
    }
    Tracker& operator=(const Tracker& init)
    {
        value_ = init.value_;
        return *this;
    }
    Tracker& operator=(Tracker&& init) noexcept
    {
        value_ = init.value_;
        init.value_ = -1;       // signing that the object is moved
        return *this;
    }
    ~Tracker()
    {
        ++destruct_count_;
    }

    // reset the counters
    static void reset()
    {
        construct_count_ = 0;
        destruct_count_ = 0;
        copy_count_ = 0;
        move_count_ = 0;
    }

    // check memory leak
    static void check_no_leak()
    {
        if (construct_count_ != destruct_count_)
        {
            std::ostringstream oss;
            oss << "Tracker leak: contruct = " << construct_count_
                << ", destruct = " << destruct_count_;
            throw std::runtime_error(oss.str());
        }
    }
};
inline int Tracker::construct_count_ = 0;
inline int Tracker::destruct_count_ = 0;
inline int Tracker::copy_count_ = 0;
inline int Tracker::move_count_ = 0;

// CopyThrower: throw exception at assigned timing, 
// which is designed to test exception throwing when 
// copying and when moving but actually calls copy
struct CopyThrower
{
    static int copy_count_;
    static int throw_after_copy_;   // -1 = don't throw

    int value_;

    // constructor, copy controller
    CopyThrower() = default;
    explicit CopyThrower(int value)
        : value_(value)
    {
    }

    CopyThrower(const CopyThrower& init)
        : value_(init.value_)
    {
        if (++copy_count_ > throw_after_copy_ && throw_after_copy_ > -1)
        {
            throw std::runtime_error("CopyThrower: copy");
        }
    }
    CopyThrower& operator=(const CopyThrower& init)
    {
        value_ = init.value_;
        return *this;
    }
    ~CopyThrower() = default;

    // reset the counters
    static void reset()
    {
        copy_count_ = 0;
        throw_after_copy_ = -1;
    }
};
inline int CopyThrower::copy_count_ = 0;
inline int CopyThrower::throw_after_copy_ = -1;

// CopyThrower: throw exception at assigned timing, 
// which is designed to test exception throwing when 
// moving
struct MoveThrower
{
    static int move_count_;
    static int throw_after_move_;

    int value_;

    // constructor, copy controller
    MoveThrower() = default;
    MoveThrower(int value)
        : value_(value)
    {
    }

    // delete copy to make move constructor not noexcept
    // while the struct is movable
    MoveThrower(const MoveThrower&) = delete;
    MoveThrower& operator=(const MoveThrower&) = delete;
    MoveThrower(MoveThrower&& init)
        : value_(init.value_)
    {
        if (++move_count_ > throw_after_move_ && throw_after_move_ > -1)
        {
            throw std::runtime_error("MoveThrower: move");
        }
        init.value_ = -1;   // signing that the object is moved
    }
    MoveThrower& operator=(MoveThrower&& init)
    {
        value_ = init.value_;
        init.value_ = -1;   // signing that the object is moved
        return *this;
    }
    ~MoveThrower() = default;

    // reset the counters
    static void reset()
    {
        move_count_ = 0;
        throw_after_move_ = -1;
    }
};
inline int MoveThrower::move_count_ = 0;
inline int MoveThrower::throw_after_move_ = -1;

// registrar
struct TestCase
{
    std::string name_;
    std::function<void()> func_;
};

inline std::vector<TestCase>& all_tests()
{
    static std::vector<TestCase> tests;
    return tests;
}

struct TestRegistrar
{
    TestRegistrar(std::string test_name, std::function<void()> function)
    {
        all_tests().push_back({test_name, std::move(function)});
    }
};

#define TEST_CASE(test) \
    void test();  \
    static TestRegistrar regIstrar_##test(#test, test);  \
    void test()

inline int run_all_tests()
{
    int passed = 0, failed = 0;

    for (auto& test : all_tests())
    {
        std::cout << "[ RUN  ]: " << test.name_ << '\n';
        try
        {
            test.func_();
            ++passed;
            std::cout << "[ PASS ]: " << test.name_ << '\n';
        }
        catch(const test_util_details::TestFailure& f)
        {
            ++failed;
            std::cerr << "[ FAILED ]: " << test.name_ << ": " << f.msg 
                      << " at " << f.file << ": " << f.line << '\n';
        }
        catch(const std::exception& e)
        {
            ++failed;
            std::cerr << "[ FAILED ]: " << test.name_ << ": " << e.what() << '\n';
        }
        catch(...)
        {
            ++failed;
            std::cerr << "[ FAILED ]: " << test.name_ << ": unknown exception\n";
        }

        std::cerr << std::endl;
    }
    std::cout << "passed: " << passed << ", failed: " << failed << std::endl;
    // std::system("PAUSE");
    return failed ? 1 : 0;
}