////////////////////////////////////////////////////////////////////////////////
/// @brief tasks used to handle periodic events
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
/// @author Achim Brandt
/// @author Copyright 2008-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_SCHEDULER_PERIODIC_TASK_H
#define TRIAGENS_SCHEDULER_PERIODIC_TASK_H 1

#include "Basics/Common.h"
#include "Scheduler/Task.h"

extern "C" {
  struct TRI_json_s;
}

namespace triagens {
  namespace rest {

////////////////////////////////////////////////////////////////////////////////
/// @ingroup Scheduler
/// @brief task used to handle periodic events
////////////////////////////////////////////////////////////////////////////////

    class  PeriodicTask : virtual public Task {
      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructs a new task for a given periodic event
////////////////////////////////////////////////////////////////////////////////

        PeriodicTask (std::string const&,
                      double,
                      double);

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief called when the timer is reached
////////////////////////////////////////////////////////////////////////////////

        virtual bool handlePeriod () = 0;

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

        ~PeriodicTask ();

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief get a task specific description in JSON format
////////////////////////////////////////////////////////////////////////////////

        virtual void getDescription (struct TRI_json_s*);

////////////////////////////////////////////////////////////////////////////////
/// @brief resets the timer
////////////////////////////////////////////////////////////////////////////////

        void resetTimer (double offset, double interval);

      protected:

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        bool setup (Scheduler*, EventLoop);

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        void cleanup ();

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        bool handleEvent (EventToken, EventType);

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief periodic event
////////////////////////////////////////////////////////////////////////////////

        EventToken watcher;

      private:
        double offset;
        double interval;
    };
  }
}

#endif
