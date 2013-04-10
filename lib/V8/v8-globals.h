////////////////////////////////////////////////////////////////////////////////
/// @brief V8-vocbase bridge
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2013 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Copyright 2011-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_V8_V8_GLOBALS_H
#define TRIAGENS_V8_V8_GLOBALS_H 1

#include "Basics/Common.h"

#include <regex.h>

#include <v8.h>

#include "Basics/ReadWriteLock.h"

// -----------------------------------------------------------------------------
// --SECTION--                                                     public macros
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief create a v8 symbol for the specified string
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_SYMBOL(name) \
  v8::String::NewSymbol(name, strlen(name))

////////////////////////////////////////////////////////////////////////////////
/// @brief create a v8 string for the specified string
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_STRING(name) \
  v8::String::New(name)

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing an exception with an error code
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_EXCEPTION(scope, code) \
  return scope.Close(v8::ThrowException(TRI_CreateErrorObject(code)))

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing an exception and returning
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_EXCEPTION_MESSAGE(scope, code, message) \
  return scope.Close(v8::ThrowException(TRI_CreateErrorObject(code, message, true)))

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing a usage exception and returning
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_EXCEPTION_USAGE(scope, usage)                           \
  do {                                                                 \
    string msg = "usage: ";                                            \
    msg += usage;                                                      \
    return scope.Close(                                                \
      v8::ThrowException(                                              \
        TRI_CreateErrorObject(TRI_ERROR_BAD_PARAMETER, msg.c_str()))); \
  }                                                                    \
  while (0)

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing an internal exception and returning
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_EXCEPTION_INTERNAL(scope, message) \
  return scope.Close(v8::ThrowException(TRI_CreateErrorObject(TRI_ERROR_INTERNAL, message)));

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing a parameter exception and returning
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_EXCEPTION_PARAMETER(scope, message) \
  return scope.Close(v8::ThrowException(TRI_CreateErrorObject(TRI_ERROR_BAD_PARAMETER, message)));

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing an out-of-memory exception and returning
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_EXCEPTION_MEMORY(scope) \
  return scope.Close(v8::ThrowException(TRI_CreateErrorObject(TRI_ERROR_OUT_OF_MEMORY)));

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing an exception for an system error
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_EXCEPTION_SYS(scope, message)                        \
  do {                                                              \
    TRI_set_errno(TRI_ERROR_SYS_ERROR);                             \
    string msg = message;                                           \
    msg += ": ";                                                    \
    msg += TRI_LAST_ERROR_STR;                                      \
    return scope.Close(v8::ThrowException(                          \
      TRI_CreateErrorObject(                                        \
        TRI_errno(),                                                \
        msg.c_str())));                                             \
  }                                                                 \
  while (0)

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing a type error
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_TYPE_ERROR(scope, message) \
  return scope.Close(v8::ThrowException(v8::Exception::TypeError(v8::String::New(message))))

////////////////////////////////////////////////////////////////////////////////
/// @brief shortcut for throwing a syntax error
////////////////////////////////////////////////////////////////////////////////

#define TRI_V8_SYNTAX_ERROR(scope, message) \
  return scope.Close(v8::ThrowException(v8::Exception::SyntaxError(v8::String::New(message))))

// -----------------------------------------------------------------------------
// --SECTION--                                                      public types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief globals stored in the isolate
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_v8_global_s {

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

  TRI_v8_global_s (v8::Isolate*);

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

  ~TRI_v8_global_s ();

// -----------------------------------------------------------------------------
// --SECTION--                                                           HELPERS
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief barrier mapping for weak pointers
////////////////////////////////////////////////////////////////////////////////

  std::map< void*, v8::Persistent<v8::Value> > JSBarriers;

// -----------------------------------------------------------------------------
// --SECTION--                                       JAVASCRIPT OBJECT TEMPLATES
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief error template
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::ObjectTemplate> ErrorTempl;

////////////////////////////////////////////////////////////////////////////////
/// @brief general cursor template
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::ObjectTemplate> GeneralCursorTempl;

////////////////////////////////////////////////////////////////////////////////
/// @brief TRI_shaped_json_t template
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::ObjectTemplate> ShapedJsonTempl;

////////////////////////////////////////////////////////////////////////////////
/// @brief transaction template
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::ObjectTemplate> TransactionTempl;

////////////////////////////////////////////////////////////////////////////////
/// @brief TRI_vocbase_col_t template
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::ObjectTemplate> VocbaseColTempl;

////////////////////////////////////////////////////////////////////////////////
/// @brief TRI_vocbase_t template
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::ObjectTemplate> VocbaseTempl;

// -----------------------------------------------------------------------------
// --SECTION--                                              JAVASCRIPT CONSTANTS
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief "DELETE" function name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> DeleteConstant;

////////////////////////////////////////////////////////////////////////////////
/// @brief "GET" function name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> GetConstant;

////////////////////////////////////////////////////////////////////////////////
/// @brief "HEAD" function name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> HeadConstant;

////////////////////////////////////////////////////////////////////////////////
/// @brief "OPTIONS" function name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> OptionsConstant;

////////////////////////////////////////////////////////////////////////////////
/// @brief "PATCH" function name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> PatchConstant;

////////////////////////////////////////////////////////////////////////////////
/// @brief "POST" function name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> PostConstant;

////////////////////////////////////////////////////////////////////////////////
/// @brief "PUT" function name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> PutConstant;

// -----------------------------------------------------------------------------
// --SECTION--                                              JAVASCRIPT KEY NAMES
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief "bodyFromFile" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> BodyFromFileKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "body" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> BodyKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "code" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> CodeKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "contentType" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> ContentTypeKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "error" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> ErrorKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "errorMessage" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> ErrorMessageKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "errorNum" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> ErrorNumKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "headers" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> HeadersKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "id" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> IdKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "isSystem" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> IsSystemKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "isVolatile" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> IsVolatileKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "journalSize" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> JournalSizeKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "keyOptions" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> KeyOptionsKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "parameters" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> ParametersKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "path" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> PathKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "prefix" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> PrefixKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "requestBody" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> RequestBodyKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "requestType" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> RequestTypeKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "responseCode" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> ResponseCodeKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "suffix" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> SuffixKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "transformations" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> TransformationsKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "url" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> UrlKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "user" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> UserKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "waitForSync" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> WaitForSyncKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "_from" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> _FromKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "_id" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> _IdKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "_key" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> _KeyKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "_oldRev" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> _OldRevKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "_rev" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> _RevKey;

////////////////////////////////////////////////////////////////////////////////
/// @brief "_to" key name
////////////////////////////////////////////////////////////////////////////////

  v8::Persistent<v8::String> _ToKey;

// -----------------------------------------------------------------------------
// --SECTION--                                               REGULAR EXPRESSIONS
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief document identifier as collection name/key
////////////////////////////////////////////////////////////////////////////////

  regex_t DocumentIdRegex;

////////////////////////////////////////////////////////////////////////////////
/// @brief document identifier
////////////////////////////////////////////////////////////////////////////////

  regex_t DocumentKeyRegex;

////////////////////////////////////////////////////////////////////////////////
/// @brief numeric id, used for index ids
////////////////////////////////////////////////////////////////////////////////

  regex_t IdRegex;

////////////////////////////////////////////////////////////////////////////////
/// @brief index identifier as collection-name:index-id
////////////////////////////////////////////////////////////////////////////////

  regex_t IndexIdRegex;

// -----------------------------------------------------------------------------
// --SECTION--                                                          DATABASE
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief currently running transaction
////////////////////////////////////////////////////////////////////////////////

  void* _currentTransaction;

////////////////////////////////////////////////////////////////////////////////
/// @brief pointer to the vocbase (TRI_vocbase_t*)
////////////////////////////////////////////////////////////////////////////////

  void* _vocbase;
}
TRI_v8_global_t;

// -----------------------------------------------------------------------------
// --SECTION--                                                  GLOBAL FUNCTIONS
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief creates a global context
////////////////////////////////////////////////////////////////////////////////

TRI_v8_global_t* TRI_CreateV8Globals(v8::Isolate*);

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a method to a prototype object
////////////////////////////////////////////////////////////////////////////////

void TRI_AddProtoMethodVocbase (v8::Handle<v8::Template> tpl,
                                const char* const name,
                                v8::Handle<v8::Value>(*func)(v8::Arguments const&),
                                const bool isHidden = false);

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a method to an object
////////////////////////////////////////////////////////////////////////////////

void TRI_AddMethodVocbase (v8::Handle<v8::ObjectTemplate> tpl,
                           const char* const name,
                           v8::Handle<v8::Value>(*func)(v8::Arguments const&),
                           const bool isHidden = false);

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a global function to the given context
////////////////////////////////////////////////////////////////////////////////

void TRI_AddGlobalFunctionVocbase (v8::Handle<v8::Context> context,
                                   const char* const name,
                                   v8::Handle<v8::Value>(*func)(v8::Arguments const&));

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a global function to the given context
////////////////////////////////////////////////////////////////////////////////

void TRI_AddGlobalFunctionVocbase (v8::Handle<v8::Context> context,
                                   const char* const name,
                                   v8::Handle<v8::Function> func);

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a global variable to the given context
////////////////////////////////////////////////////////////////////////////////

void TRI_AddGlobalVariableVocbase (v8::Handle<v8::Context> context,
                                   const char* const name,
                                   v8::Handle<v8::Value> value);

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
