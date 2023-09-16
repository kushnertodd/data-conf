#include <algorithm>
#include "bdb_dao.hpp"
#include "order_dto.hpp"
#include "order_item_dto.hpp"
#include "item_dao.hpp"
#include "item_dto.hpp"
#include "order_item_dao.hpp"

/*!
 * @brief save order_item dto to order_item database
 * @param order_item_db order_item database to which to save
 * @param order_item_dto prinicpals dto to save
 * @param errors if save fails
 */
void Order_item_DAO::delete_order(Bdb_dbp &order_db,
                                  Bdb_dbp &order_item_db,
                                  Bdb_dbp &order_item_order_id_sdb,
                                  const std::string &order_id,
                                  Order_DTO &order_dto,
                                  Bdb_errors &errors) {
  Order_item_DTO_key_list order_item_dto_key_list;
  Order_item_DAO::select_by_order_sdb(order_item_order_id_sdb,
                                      order_id,
                                      order_item_dto_key_list,
                                      errors);
  if (!errors.has()) {
    for (const Order_item_DTO_key &order_item_dto_key: order_item_dto_key_list.list) {
      Bdb_DAO::delete_key<Order_item_DTO_key>(order_item_db,
                                              order_item_dto_key,
                                              errors, nullptr, true);
      if (errors.has())
        break;
    }
    Order_DTO_key order_dto_key(order_id);
    if (!errors.has())
      Bdb_DAO::lookup<Order_DTO_key, Order_DTO>(order_db,
                                                order_dto_key,
                                                order_dto,
                                                errors, true);
    if (!errors.has())
      Bdb_DAO::delete_key<Order_DTO_key>(order_db,
                                         order_dto_key,
                                         errors, nullptr, true);
  }
}

void Order_item_DAO::delete_order_item(Bdb_dbp &order_item_db,
                                       Bdb_dbp &order_item_order_id_sdb,
                                       Bdb_dbp &order_item_item_id_sdb,
                                       const std::string &order_id,
                                       const std::string &item_id,
                                       Order_item_DTO &order_item_dto_with_key,
                                       Bdb_errors &errors) {
  Order_item_DTO_key_list order_item_dto_key_by_order_id_list;
  Order_item_DAO::select_by_order_sdb(order_item_order_id_sdb,
                                      order_id,
                                      order_item_dto_key_by_order_id_list,
                                      errors);
  if (!errors.has()) {
    Order_item_DTO_key_list order_item_dto_key_by_item_id_list;
    Order_item_DAO::select_by_item_sdb(order_item_item_id_sdb,
                                       item_id,
                                       order_item_dto_key_by_item_id_list,
                                       errors);
    if (!errors.has()) {
      std::map<std::string, bool> order_item_id_map;
      for (const Order_item_DTO_key &order_item_DTO_key: order_item_dto_key_by_order_id_list.list) {
        order_item_id_map[order_item_DTO_key.order_item_id] = true;
      }
      Order_item_DTO_key_list matched_order_item_dto_key_list;
      for (const Order_item_DTO_key &order_item_DTO_key: order_item_dto_key_by_item_id_list.list) {
        if (order_item_id_map[order_item_DTO_key.order_item_id]) {
          Order_item_DTO_key order_item_DTO_key_match(order_item_DTO_key.order_item_id);
          matched_order_item_dto_key_list.list.push_back(order_item_DTO_key_match);
        }
      }
      if (matched_order_item_dto_key_list.list.empty())
        errors.add("Order_item_DAO::delete_order_item", "1", "no order_item matching order_id " +
            order_id + ", item_id " + item_id);
      else if (matched_order_item_dto_key_list.list.size() > 1) {
        // TODO: kludge
        //        errors.add("Order_item_DAO::delete_order_item", "1", "too many order_items matching order_id " +
        //            order_id + ", item_id " + item_id);
        Bdb_DAO::lookup<Order_item_DTO_key, Order_item_DTO>(order_item_db,
                                                            matched_order_item_dto_key_list.list.front(),
                                                            order_item_dto_with_key,
                                                            errors);
        if (!errors.has())
          Bdb_DAO::delete_key<Order_item_DTO_key>(order_item_db,
                                                  matched_order_item_dto_key_list.list.front(),
                                                  errors, nullptr, true);
      } else {
        Bdb_DAO::lookup<Order_item_DTO_key, Order_item_DTO>(order_item_db,
                                                            matched_order_item_dto_key_list.list.front(),
                                                            order_item_dto_with_key,
                                                            errors);
        if (!errors.has())
          Bdb_DAO::delete_key<Order_item_DTO_key>(order_item_db,
                                                  matched_order_item_dto_key_list.list.front(),
                                                  errors, nullptr, true);
      }
    }
  }
}

/*!
 * @brief load order_item database from delimited record file
 * @param order_item_db order_item database to which to save
 * @param text_file order_item records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Order_item_DAO::load(Bdb_dbp &order_item_db,
                         const std::string &text_file,
                         Bdb_errors &errors,
                         char delimiter) {
  return Bdb_DAO::load<Order_item_DTO_key, Order_item_DTO>
      (order_item_db, text_file, errors, delimiter);
}

/*!
 * @brief select order_item from database based on order_item id
 * @param order_item_db order_item database to which to save
 * @param order_item_id order_item record key
 * @param order_item_dto order_item dto to save
 * @param errors if save fails
 */
void Order_item_DAO::lookup(Bdb_dbp &order_item_db,
                            const std::string &order_item_id,
                            Order_item_DTO &order_item_dto,
                            Bdb_errors &errors) {
  Order_item_DTO_key order_item_dto_key(order_item_id);
  Bdb_DAO::lookup<Order_item_DTO_key, Order_item_DTO>
      (order_item_db, order_item_dto_key, order_item_dto, errors);
}

/*!
 * @brief save order_item dto to order_item database
 * @param order_item_db order_item database to which to save
 * @param order_item_dto prinicpals dto to save
 * @param errors if save fails
 */
void Order_item_DAO::save(Bdb_dbp &order_item_db,
                          Order_item_DTO &order_item_dto,
                          Order_item_DTO &order_item_dto_with_key,
                          Bdb_errors &errors) {
  Bdb_DAO::save_key<Order_item_DTO_key, Order_item_DTO>
      (order_item_db, order_item_dto, order_item_dto_with_key, errors);
}

/*!
 * @brief select order_item key list using order id to search order id->order_item key secondary database
 * @param order_item_order_id_sdb order id->order_item key secondary database
 * @param order_id secondary database search key
 * @param order_item_dto_key_list selected order_item key list
 * @param errors if order key not found
 */
void Order_item_DAO::select_by_item_sdb(Bdb_dbp &order_item_item_id_sdb,
                                        const std::string &item_id,
                                        Order_item_DTO_key_list &order_item_dto_key_list,
                                        Bdb_errors &errors) {
  Item_DTO_key item_dto_key(item_id);
  Bdb_cursor bdb_cursor(order_item_item_id_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Item_DTO_key,
                                      Order_item_DTO_key,
                                      Order_item_DTO_key_list>
        (item_dto_key,
         order_item_dto_key_list,
         errors, true);
}

/*!
 * @brief select order_item key list using order id to search order id->order_item key secondary database
 * @param order_item_order_id_sdb order id->order_item key secondary database
 * @param order_id secondary database search key
 * @param order_item_dto_key_list selected order_item key list
 * @param errors if order key not found
 */
void Order_item_DAO::select_by_order_sdb(Bdb_dbp &order_item_order_id_sdb,
                                         const std::string &order_id,
                                         Order_item_DTO_key_list &order_item_dto_key_list,
                                         Bdb_errors &errors) {
  Order_DTO_key order_dto_key(order_id);
  Bdb_cursor bdb_cursor(order_item_order_id_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Order_DTO_key,
                                      Order_item_DTO_key,
                                      Order_item_DTO_key_list>
        (order_dto_key,
         order_item_dto_key_list,
         errors, true);
}

/*!
 * @brief select order_item key list using order id to search order id->order_item key secondary database
 * @param order_item_order_id_sdb order id->order_item key secondary database
 * @param order_id secondary database search key
 * @param order_item_dto_key_list selected order_item key list
 * @param errors if order key not found
 */
void Order_item_DAO::select_by_key_list(Bdb_dbp &order_item_db,
                                        Order_item_DTO_key_list &order_item_dto_key_list,
                                        Order_item_DTO_list &order_item_dto_list,
                                        Bdb_errors &errors) {
  Bdb_DAO::select_by_key_list<Order_item_DTO_key,
                              Order_item_DTO_key_list,
                              Order_item_DTO,
                              Order_item_DTO_list>
      (order_item_db,
       order_item_dto_key_list,
       order_item_dto_list,
       errors);
}

/*!
 * @brief select all item records corresponding to order id via order id->order_item key secondary database
 * @param order_item_order_id_sdb order id->order_item key secondary database
 * @param order_item_db order_item database
 * @param item_db items database
 * @param order_id secondary database search key
 * @param item_dto_list selected items list
 * @param errors if order key not found
 */
void Order_item_DAO::select_items_for_order(Bdb_dbp &order_item_db,
                                            Bdb_dbp &item_db,
                                            Bdb_dbp &order_item_order_id_sdb,
                                            const std::string &order_id,
                                            Item_DTO_list &item_dto_list,
                                            Bdb_errors &errors) {
  Order_item_DTO_key_list order_item_dto_key_list;
  select_by_order_sdb(order_item_order_id_sdb, order_id, order_item_dto_key_list, errors);
  Order_item_DTO_list order_item_dto_list;
  if (!errors.has())
    select_by_key_list(order_item_db, order_item_dto_key_list, order_item_dto_list, errors);
  if (!errors.has())
    select_item_list(item_db, order_item_dto_list, item_dto_list, errors);
}

/*!
 * @brief select item dto list using order_item dto list
 * @param item_db item primary database
 * @param order_item_dto_list select one item dto per order_item dto
 * @param item_dto_list selected item dto list
 * @param errors if order_item key not found
 */
void Order_item_DAO::select_item_list(Bdb_dbp &item_db,
                                      Order_item_DTO_list &order_item_dto_list,
                                      Item_DTO_list &item_dto_list,
                                      Bdb_errors &errors) {
  for (Order_item_DTO &order_item_dto: order_item_dto_list.list) {
    Item_DTO_key item_dto_key(order_item_dto);
    Item_DTO item_dto;
    Item_DAO::lookup(item_db, item_dto_key.item_id, item_dto, errors);
    if (!errors.has())
      item_dto_list.add(item_dto);
    if (errors.has())
      break;
  }
}

/*!
 * @brief select all order records corresponding to item id via item id->order_item key secondary database
 * @param order_item_item_id_sdb item id->order_item key secondary database
 * @param order_item_db order_item database
 * @param order_db orders database
 * @param item_id secondary database search key
 * @param order_dto_list selected orders list
 * @param errors if item key not found
 */
void Order_item_DAO::select_orders_for_item(Bdb_dbp &order_item_item_id_sdb,
                                            Bdb_dbp &order_item_db,
                                            Bdb_dbp &order_db,
                                            const std::string &item_id,
                                            Order_DTO_list &order_dto_list,
                                            Bdb_errors &errors) {
  Order_item_DTO_key_list order_item_dto_key_list;
  select_by_item_sdb(order_item_item_id_sdb, item_id, order_item_dto_key_list, errors);
  Order_item_DTO_list order_item_dto_list;
  if (!errors.has())
    select_by_key_list(order_item_db, order_item_dto_key_list, order_item_dto_list, errors);
  if (!errors.has())
    select_order_list(order_db, order_item_dto_list, order_dto_list, errors);
}

/*!
 * @brief select order dto list using order_item dto list
 * @param order_db order primary database
 * @param order_item_dto_list select one order dto per order_item dto
 * @param order_dto_list selected order dto list
 * @param errors if order_item key not found
 */
void Order_item_DAO::select_order_list(Bdb_dbp &order_db,
                                       Order_item_DTO_list &order_item_dto_list,
                                       Order_DTO_list &order_dto_list,
                                       Bdb_errors &errors) {
  for (Order_item_DTO &order_item_dto: order_item_dto_list.list) {
    Order_DTO_key order_dto_key(order_item_dto);
    Order_DTO order_dto;
    Bdb_DAO::lookup<Order_DTO_key, Order_DTO>
        (order_db, order_dto_key, order_dto, errors);
    if (!errors.has())
      order_dto_list.add(order_dto);
    else
      break;
  }
}

/*!
 * @brief select order dto list using order_item dto list
 * @param order_db order primary database
 * @param order_item_dto_list select one order dto per order_item dto
 * @param order_dto_list selected order dto list
 * @param errors if order_item key not found
 */
void Order_item_DAO::select_other_items(Bdb_dbp &order_item_order_id_sdb,
                                        Bdb_dbp &order_item_db,
                                        Bdb_dbp &item_db,
                                        const std::string &order_id,
                                        Item_DTO_list &item_dto_list,
                                        Bdb_errors &errors) {
  Item_DTO_list items_in_order_dto_list;
  Order_item_DAO::select_items_for_order(order_item_db,
                                         item_db,
                                         order_item_order_id_sdb,
                                         order_id,
                                         items_in_order_dto_list,
                                         errors);
  if (!errors.has())
    Item_DAO::select_all(item_db, item_dto_list, errors);
  if (!errors.has()) {
    for (auto &item_dto: items_in_order_dto_list.list) {
      auto it = std::remove_if(item_dto_list.list.begin(),
                               item_dto_list.list.end(),
                               [&item_dto](Item_DTO &dto) {
                                 return item_dto.item_id == dto.item_id;
                               });
      item_dto_list.list.erase(it, item_dto_list.list.end());
    }
  }
}
