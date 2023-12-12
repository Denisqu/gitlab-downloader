#ifndef __DBMANAGER_H__
#define __DBMANAGER_H__

#include "dblib-export.hpp"
#include <memory>

/*
Перед написанием этого класса нужно досмотреть
курс по архитектуре ПО от CSC.
*/
template <typename DBHandlerT> class DBLIB_EXPORT DBManager {

public:
  explicit DBManager();

private:
  DBHandlerT *createDBHandler();

  std::unique_ptr<DBHandlerT> m_DBHandler = nullptr;
};

#endif // __DBMANAGER_H__