// -*- C++ -*-
// $Id$

#ifndef ACE_VECTOR_T_CPP
#define ACE_VECTOR_T_CPP

#define ACE_BUILD_DLL

#include "ace/Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Vector.h"

#if !defined(__ACE_INLINE__)
#include "ace/Vector.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Vector, "$Id$")


template <class T>
ACE_Vector<T>::ACE_Vector (size_t size,
                           ACE_Allocator *alloc)
  : size_ (0),
    capacity_ (size),
    allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (size > 0)
    {
      ACE_NEW_MALLOC (this->vector_,
                      (T *) this->allocator_->malloc (size * sizeof (T)),
                      T);
      for (size_t i = 0; i < size; ++i)
        new (&vector_[i]) T;
    }
  else
    this->vector_ = 0;
}

template <class T>
ACE_Vector<T>::ACE_Vector (size_t size,
                           const T &default_value,
                           ACE_Allocator *alloc)
  : size_ (size),
    capacity_ (size),
    allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  if (size > 0)
    {
      ACE_NEW_MALLOC (this->vector_,
                      (T *) this->allocator_->malloc (size * sizeof (T)),
                      T);

      for (size_t i = 0; i < size; ++i)
        new (&vector_[i]) T (default_value);
    }
  else
    this->vector_ = 0;
}

template <class T>
ACE_Vector<T>::ACE_Vector (const ACE_Vector<T> &s)
   : size_ (s.size ()),
     capacity_ (s.capacity ()),
     allocator_ (s.allocator_)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  ACE_NEW_MALLOC (this->vector_,
                  (T *) this->allocator_->malloc (s.capacity () * sizeof (T)),
                  T);

  for (size_t i = 0; i < this->capacity (); ++i)
    new (&this->vector_[i]) T (s.vector_[i]);
}

template <class T>
ACE_Vector<T>::ACE_Vector (ACE_Vector<T>::iterator first,
                           ACE_Vector<T>::iterator last,
                           ACE_Allocator *alloc)
  : size_ (0),
    capacity_ (0),
    allocator_ (alloc)
{
  if (this->allocator_ == 0)
    this->allocator_ = ACE_Allocator::instance ();

  const size_t size = last - first;

  if (size > 0)
    {
      ACE_NEW_MALLOC (this->vector_,
                      (T *) this->allocator_->malloc (size * sizeof (T)),
                      T);

      T* temp = this->vector_;

      for (ACE_Vector<T>::iterator i = first; i != last; ++i)
          new (temp++) T (*i);

      this->size_ = size;
      this->capacity_ = size;
    }
  else
    this->vector_ = 0;
}

template <class T> void
ACE_Vector<T>::swap (ACE_Vector<T>& v)
{
  const size_t this_size = this->size_;
  const size_t v_size    = v.size ();
  size_t num_elements    = v_size;

  // Expand the smaller of the two vectors, if necessary
  if (this->size_ > v_size)
    {
      v.reserve (this->size_);
      num_elements = this->size_;
    }
  else if (v_size > this->size_)
    {
      this->reserve (v_size);
      num_elements = v_size;
    }

  // Do the swap
  for (size_t i = 0; i < num_elements; ++i)
    {
      T temp = this->vector_[i];

      if (i < v_size && i >= this_size)
        this->push_back (v[i]);
      else if (i < v_size)
        this->vector_[i] = v[i];

      if (i < this_size && i >= v_size)
        v.push_back (temp);
      else if (i < this_size)
        v[i] = temp;
    }

  // Now reduce sizes if necessary
  if (this_size < v_size)
    for (size_t j = this_size; j < v_size; ++j)
      v.pop_back ();

  if (v_size < this_size)
    for (size_t k = v_size; k < this_size; ++k)
      this->pop_back ();
}

template <class T> ACE_Vector<T>::iterator
ACE_Vector<T>::insert (ACE_Vector<T>::iterator pos, const T& x)
{
  size_t diff = pos - this->begin ();

  if (diff >= 0 && diff < this->size_)
    {
      this->grow ();  // Only grows when necessary

      // If memory was reallocated then the pos iterator that was
      // passed in is no longer valid, so recompute it.
      pos = this->begin () + diff;

      // Move all elements after pos - 1;
      for (ACE_Vector<T>::iterator i = this->end ();
           i != pos;
           *(i--) = *i)
        continue;

      ++(this->size_);

      // Now do the insertion
      *pos = x;
    }

  return this->begin () + diff;
}

template <class T> void
ACE_Vector<T>::insert (ACE_Vector<T>::iterator pos,
                       ACE_Vector<T>::iterator first,
                       ACE_Vector<T>::iterator last)
{
  size_t diff  = pos - this->begin ();
  size_t range = last - first;

  if (range > 0 && diff >= 0 && diff < this->size_)
    {
      this->grow (range);  // Only grows when necessary

      // If memory was reallocated then the pos iterator that was
      // passed in is no longer valid, so recompute it.
      pos = this->begin () + diff;

      // Move all elements after pos - 1;
      for (ACE_Vector<T>::iterator i = this->end () + range - 1;
           i != pos;
           --i)
        *i = *(i - range);

      this->size_ += range;

      // Now do the insertion
      for (ACE_Vector<T>::iterator j = pos; j != pos + range; ++j, ++first)
        *j = *first;
    }
}

template <class T> void
ACE_Vector<T>::insert (ACE_Vector<T>::iterator pos, 
                       size_t range,
                       const T& x)
{
  size_t diff  = pos - this->begin ();

  if (range > 0 && diff >= 0 && diff < this->size_)
    {
      this->grow (range);  // Only grows when necessary

      // If memory was reallocated then the pos iterator that was
      // passed in is no longer valid, so recompute it.
      pos = this->begin () + diff;

      // Move all elements after pos - 1;
      for (ACE_Vector<T>::iterator i = this->end () + range - 1;
           i != pos;
           --i)
        *i = *(i - range);

      this->size_ += range;

      // Now do the insertion
      for (ACE_Vector<T>::iterator j = pos; j != pos + range; ++j)
        *j = x;
    }
}

template <class T> ACE_Vector<T>::iterator
ACE_Vector<T>::erase (ACE_Vector<T>::iterator pos)
{
  size_t diff  = pos - this->begin ();

  if (diff >= 0 && diff < this->size_)
    {
      // Move all elements after pos - 1;
      for (ACE_Vector<T>::iterator i = pos; i != this->end (); *(i++) = *i)
        continue;

      --(this->size_);
    }

  return pos;
}

template <class T> ACE_Vector<T>::iterator
ACE_Vector<T>::erase (ACE_Vector<T>::iterator first,
                      ACE_Vector<T>::iterator last)
{
  size_t diff  = first - this->begin ();
  size_t range = last - first;

  if (range > 0 && diff >= 0 && diff < this->size_)
    {
      // Move all elements after pos - 1;
      for (ACE_Vector<T>::iterator i = first; i + range != this->end (); ++i)
        *i = *(i + range);

      this->size_ -= range;
    }

  return first;
}

template <class T> int
ACE_Vector<T>::reserve (size_t new_size)
{
  if (new_size > this->capacity_)
    {
      if (this->capacity_ > 0)
        {
          // Make sure growth is proportional to the capacity.
          const size_t remainder = new_size % this->capacity_;
          if (remainder != 0)
            new_size += this->capacity_ - remainder;
        }

      T *tmp;

      ACE_NEW_MALLOC_RETURN (tmp,
                             (T *) this->allocator_->malloc (new_size *
                                                             sizeof (T)),
                             T,
                             -1);

      for (size_t i = 0; i < this->size_; ++i)
         new (&tmp[i]) T (this->vector_[i]);

      // Initialize the new portion of the array that exceeds the
      // previously allocated section.
      for (size_t j = this->size_; j < new_size; ++j)
        new (&tmp[j]) T;

      ACE_DES_ARRAY_FREE (this->vector_,
                          this->capacity_,
                          this->allocator_->free,
                          T);

      this->vector_ = tmp;
      this->capacity_ = new_size;
    }

  return 0;
}

template <class T> void
ACE_Vector<T>::operator= (const ACE_Vector<T> &rhs)
{
  // Check for "self-assignment".

  if (this != &rhs)
    {
      if (this->capacity_ < rhs.capacity ())
        {
          ACE_DES_ARRAY_FREE (this->vector_,
                              this->capacity_,
                              this->allocator_->free,
                              T);

          ACE_NEW_MALLOC (this->vector_,
                          (T *) this->allocator_->malloc (rhs.capacity () *
                                                          sizeof (T)),
                          T);
          this->capacity_ = rhs.capacity ();
        }

      this->size_ = rhs.size ();
      this->capacity_ = rhs.capacity ();

      for (size_t i = 0; i < this->size_; ++i)
        new (&this->vector_[i]) T (rhs.vector_[i]);
    }
}

template <class T> int
ACE_Vector<T>::operator== (const ACE_Vector<T>& rhs) const
{
  if (this == &rhs)
    return 1;
  else if (this->size_ != rhs.size ())
    return 0;

  ACE_Vector<T>::iterator lhs_first = this->vector_;
  ACE_Vector<T>::iterator rhs_first = rhs.begin ();
  ACE_Vector<T>::iterator rhs_last  = rhs.end ();

  for (; rhs_first != rhs_last; ++lhs_first, ++rhs_first)
    if (*lhs_first != *rhs_first)
      return 0;

  return 1;
}

template <class T> int
ACE_Vector<T>::operator< (const ACE_Vector<T>& rhs) const
{
  if (this == &rhs)
    return 0;

  ACE_Vector<T>::iterator lhs_first = this->begin ();
  ACE_Vector<T>::iterator lhs_last  = this->end ();
  ACE_Vector<T>::iterator rhs_first = rhs.begin ();
  ACE_Vector<T>::iterator rhs_last  = rhs.end ();

  for (;
       lhs_first != lhs_last && rhs_first != rhs_last;
       ++lhs_first, ++rhs_first)
    {
      if (*lhs_first < *rhs_first)
        return 1;
      if (*rhs_first < *lhs_first)
        return 0;
    }

  return lhs_first == lhs_last && rhs_first != rhs_last;
}

#endif /* ACE_VECTOR_T_CPP */
