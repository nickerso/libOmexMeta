# ifndef SEMSIM_URI_H_
# define SEMSIM_URI_H_

# include "semsim/Preproc.h"

# include <string>

namespace semsim {

    /**
     * A URI class used in semgen.
     * @param uri_encoding A string encoding of the URI.
     */
    class SEMSIM_PUBLIC URI {
      public:
        URI(const std::string& uri)
          : encoded_uri_(uri) {}

        # if __cplusplus >= 201103L
        URI(std::string&& uri)
          : encoded_uri_(std::move(uri)) {}
        # endif

        std::string toString() const {
          return encoded_uri_;
        }

      protected:
        std::string encoded_uri_;
    };

}

# endif