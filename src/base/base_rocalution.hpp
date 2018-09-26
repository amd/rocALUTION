/* ************************************************************************
 * Copyright 2018 Advanced Micro Devices, Inc.
 *
 * ************************************************************************ */

#ifndef ROCALUTION_BASE_HPP_
#define ROCALUTION_BASE_HPP_

#include "backend_manager.hpp"

#include <complex>
#include <vector>

namespace rocalution {

template <typename ValueType>
class GlobalVector;
template <typename ValueType>
class GlobalMatrix;
class ParallelManager;

/** \private */
class RocalutionObj
{
    public:
    RocalutionObj();
    virtual ~RocalutionObj();

    virtual void Clear() = 0;

    protected:
    size_t global_obj_id;
};

// Global data for all ROCALUTION objects
/** \private */
struct Rocalution_Object_Data
{
    std::vector<class RocalutionObj*> all_obj;
};

// Global obj tracking structure
/** \private */
extern struct Rocalution_Object_Data Rocalution_Object_Data_Tracking;

/** \class BaseRocalution
  * \brief Base class for all operators and vectors
  *
  * \tparam ValueType - can be int, float, double, std::complex<float> and
  *                     std::complex<double>
  */
template <typename ValueType>
class BaseRocalution : public RocalutionObj
{
    public:
    BaseRocalution();
    /** \private */
    BaseRocalution(const BaseRocalution<ValueType>& src);
    virtual ~BaseRocalution();

    /** \private */
    BaseRocalution<ValueType>& operator=(const BaseRocalution<ValueType>& src);

    /** \brief Move the object to the accelerator backend */
    virtual void MoveToAccelerator(void) = 0;

    /** \brief Move the object to the host backend */
    virtual void MoveToHost(void) = 0;

    /** \brief Move the object to the accelerator backend with async move */
    virtual void MoveToAcceleratorAsync(void);

    /** \brief Move the object to the host backend with async move */
    virtual void MoveToHostAsync(void);

    /** \brief Sync (the async move) */
    virtual void Sync(void);

    /** \brief Clone the Backend descriptor from another object */
    virtual void CloneBackend(const BaseRocalution<ValueType>& src);

    // Clone the backend descriptor from another object with different
    // template ValueType
    template <typename ValueType2>
    void CloneBackend(const BaseRocalution<ValueType2>& src); /**< \private */

    /** \brief Print the object information (properties, backends) */
    virtual void Info(void) const = 0;

    /** \brief Clear (free all data) the object */
    virtual void Clear(void) = 0;

    protected:
    /** \brief Name of the object */
    std::string object_name_;

    // Parallel Manager
    const ParallelManager* pm_; /**< \private */

    // Backend descriptor
    Rocalution_Backend_Descriptor local_backend_; /**< \private */

    /** \brief Return true if the object is on the host */
    virtual bool is_host(void) const = 0;

    /** \brief Return true if the object is on the accelerator */
    virtual bool is_accel(void) const = 0;

    // active async transfer
    bool asyncf; /**< \private */

    friend class BaseRocalution<double>;
    friend class BaseRocalution<float>;
    friend class BaseRocalution<std::complex<double>>;
    friend class BaseRocalution<std::complex<float>>;

    friend class BaseRocalution<int>;

    friend class GlobalVector<int>;
    friend class GlobalVector<float>;
    friend class GlobalVector<double>;

    friend class GlobalMatrix<int>;
    friend class GlobalMatrix<float>;
    friend class GlobalMatrix<double>;
};

} // namespace rocalution

#endif // ROCALUTION_LOCAL_BASE_HPP_
