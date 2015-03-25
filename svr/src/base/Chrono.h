#ifndef Chrono_h__
#define Chrono_h__

// Related to <ratio> - minimal to be able to support chrono.
typedef long long __intmax_t;

/// Minimal implementation of the @c ratio class. This class provides enough
/// functionality to implement some basic @c chrono classes.
template <__intmax_t N, __intmax_t D = 1> class ratio {
public:
    static double _as_double() { return double(N) / double(D); }
};

/// Minimal implementation of the @c chrono namespace.
/// The @c chrono namespace provides types for specifying time intervals.
namespace chrono {

    /// Duration template class. This class provides enough functionality to
    /// implement @c this_thread::sleep_for().
    template <class _Rep, class _Period = ratio<1> > class duration {
    private:
        _Rep rep_;
    public:
        typedef _Rep rep;
        typedef _Period period;

        /// Construct a duration object with the given duration.
        template <class _Rep2>
        explicit duration(const _Rep2& r) : rep_(r) {};

        /// Return the value of the duration object.
        rep count() const
        {
            return rep_;
        }
    };

    // Standard duration types.
    typedef duration<__intmax_t, ratio<1, 1000000000> > nanoseconds; ///< Duration with the unit nanoseconds.
    typedef duration<__intmax_t, ratio<1, 1000000> > microseconds;   ///< Duration with the unit microseconds.
    typedef duration<__intmax_t, ratio<1, 1000> > milliseconds;      ///< Duration with the unit milliseconds.
    typedef duration<__intmax_t> seconds;                            ///< Duration with the unit seconds.
    typedef duration<__intmax_t, ratio<60> > minutes;                ///< Duration with the unit minutes.
    typedef duration<__intmax_t, ratio<3600> > hours;                ///< Duration with the unit hours.
}

#endif // Chrono_h__
