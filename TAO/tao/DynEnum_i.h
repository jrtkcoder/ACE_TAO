/* -*- C++ -*- */
// $Id$

// =======================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynEnum_i.h
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_DYNENUM_I_H
#define TAO_DYNENUM_I_H

#include "ace/Containers.h"

class TAO_Export TAO_DynEnum_i : public POA_CORBA::DynEnum
{
  // = TITLE
  //    TAO_DynEnum_i
  //
  // = DESCRIPTION
  //    Implementation of Dynamic Any type for enums
  //
public:
  TAO_DynEnum_i (const CORBA_Any& any);
  // constructor taking an Any argument

  TAO_DynEnum_i (CORBA_TypeCode_ptr tc);
  // constructor taking a typecode argument

  ~TAO_DynEnum_i (void);
  // destructor

  // Functions specific to DynEnum
  // Get and set methods for enum's string and numeric value

  char* value_as_string (CORBA::Environment& env);

  void value_as_string (const char* value_as_string,
                        CORBA::Environment& env);

  CORBA::ULong value_as_ulong (CORBA::Environment&);

  void value_as_ulong (CORBA::ULong value_as_ulong,
                       CORBA::Environment& env);

  // Functions common to all Dynamic Any types

  void assign (CORBA_DynAny_ptr dyn_any,
               CORBA::Environment &env);
  // Sets the value of the receiver to the value of the argument

  CORBA_DynAny_ptr copy (CORBA::Environment &env);
  // Creates a copy

  void destroy (CORBA::Environment &env);
  // Makes sure the contents are disposed of correctly

  void from_any (const CORBA::Any& any,
                 CORBA::Environment &env);
  // Assigns value from an Any argument

  CORBA::Any_ptr to_any (CORBA::Environment &env);
  // Outputs contents as an Any

  CORBA::TypeCode_ptr type (CORBA::Environment &);
  // Returns the (constant) type

  CORBA_DynAny_ptr current_component (CORBA::Environment &env);
  // Used in iterating through the contents

  CORBA::Boolean next (CORBA::Environment &);
  // Returns next component

  CORBA::Boolean seek (CORBA::Long index, 
                       CORBA::Environment &);
  // Jump to component at <index>

  void rewind (CORBA::Environment &);
  // Makes first component the current one

  // Insert and get functions

  // In DynEnum, these have no pertinence, so since spec requires
  // their presence, they will just throw an exception

  void insert_boolean (CORBA::Boolean value,
                       CORBA::Environment &env);
  void insert_octet (CORBA::Octet value,
                     CORBA::Environment &env);
  void insert_char (CORBA::Char value,
                    CORBA::Environment &env);
  void insert_short (CORBA::Short value,
                     CORBA::Environment &env);
  void insert_ushort (CORBA::UShort value,
                      CORBA::Environment &env);
  void insert_long (CORBA::Long value,
                    CORBA::Environment &env);
  void insert_ulong (CORBA::ULong value,
                     CORBA::Environment &env);
  void insert_float (CORBA::Float value,
                     CORBA::Environment &env);
  void insert_double (CORBA::Double value,
                      CORBA::Environment &env);
  void insert_string (const char * value,
                      CORBA::Environment &env);
  void insert_reference (CORBA::Object_ptr value,
                         CORBA::Environment &env);
  void insert_typecode (CORBA::TypeCode_ptr value,
                        CORBA::Environment &env);
  void insert_longlong (CORBA::LongLong value,
                        CORBA::Environment &env);
  void insert_ulonglong (CORBA::ULongLong value,
                         CORBA::Environment &env);
  void insert_wchar (CORBA::WChar value,
                      CORBA::Environment &env);
  void insert_any (const CORBA::Any& value,
                   CORBA::Environment &env);
  CORBA::Boolean get_boolean (CORBA::Environment &env);

  CORBA::Octet get_octet (CORBA::Environment &env);

  CORBA::Char get_char (CORBA::Environment &env);

  CORBA::Short get_short (CORBA::Environment &env);

  CORBA::UShort get_ushort (CORBA::Environment &env);

  CORBA::Long get_long (CORBA::Environment &env);

  CORBA::ULong get_ulong (CORBA::Environment &env);

  CORBA::Float get_float (CORBA::Environment &env);

  CORBA::Double get_double (CORBA::Environment &env);

  char * get_string (CORBA::Environment &env);

  CORBA::Object_ptr get_reference (CORBA::Environment &env);

  CORBA::TypeCode_ptr get_typecode (CORBA::Environment &env);

  CORBA::LongLong get_longlong (CORBA::Environment &env);

  CORBA::ULongLong get_ulonglong (CORBA::Environment &env);

  CORBA::WChar get_wchar (CORBA::Environment &env);

  CORBA::Any_ptr get_any (CORBA::Environment &env);

private:
  CORBA::TypeCode_var type_;
  // Holds the typecode of the enum

  CORBA::ULong value_;
  // Current numeric value of the enum

  TAO_DynEnum_i (void);
  // Must be instantiated with at least a typecode

  // Use copy() or assign() instead of these
  TAO_DynEnum_i (const TAO_DynEnum_i &src);
  TAO_DynEnum_i &operator= (const TAO_DynEnum_i &src);
};

#endif /* TAO_DYNENUM_I_H */
