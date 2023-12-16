#ifndef __DBMANAGER_H__
#define __DBMANAGER_H__

#include "dblib-export.hpp"
#include <memory>

/*
Класс, инкапсулирующий логику работы с БД<ТипБД>
*/
template <typename DBHandlerT> class DBLIB_EXPORT DBManager {

public:
  explicit DBManager();

private:
  DBHandlerT *createDBHandler();

  std::unique_ptr<DBHandlerT> m_DBHandler;
};

#endif // __DBMANAGER_H__