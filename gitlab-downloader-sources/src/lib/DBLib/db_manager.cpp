#include "db_manager.hpp"

// only for debug:
class IDBHandler {
  virtual void handle() = 0;
};

class SQLiteHandeler : IDBHandler {
  void handle() override;
};

template <typename DBHandlerT> DBManager<DBHandlerT>::DBManager() {
  static_assert(std::is_base_of<IDBHandler, DBHandlerT>::value,
                "DBHandler is not derived from IDBHandler");
}
