#ifndef BUBBLEERROR_H
#define BUBBLEERROR_H
#include <stdexcept>

class BubbleError: virtual public std::runtime_error {
    
protected:

    int error_number;
    int error_offset;
    
public:

    /** 
     *  Constructor that creates a runtime error based on the source.
     *  TODO: Change err_num to additional string args or add an error num system.
     *  @param msg      - The error message
     *  @param err_num  - The error number/code
     *  @param err_off  - Error offset
     */
    explicit 
    BubbleError(const std::string msg, int err_num, int err_off=0):
        std::runtime_error(msg)
        {
            error_number = err_num;
            error_offset = err_off;
            
        }

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~BubbleError() throw () {}
    
    /** Returns error number.
     *  @return #error_number
     */
    virtual int getErrorNumber() const throw() {
        return error_number;
    }
    
    /**Returns error offset.
     * @return #error_offset
     */
    virtual int getErrorOffset() const throw() {
        return error_offset;
    }

};

#endif