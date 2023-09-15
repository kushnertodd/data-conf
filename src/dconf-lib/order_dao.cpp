#include "bdb_dao.hpp"
#include "item_dto.hpp"
#include "order_dto.hpp"
#include "order_dao.hpp"

/*!
 * @brief load order database from delimited record file
 * @param order_db order database to which to save
 * @param text_file order records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Order_DAO::load(Bdb_dbp &order_db,
                    Bdb_dbp &order_bdb_triplet_db,
                    const std::string &text_file,
                    Bdb_errors &errors,
                    char delimiter) {
  return Bdb_DAO::load_triplets<Order_DTO_key,
                                Order_DTO>(order_db,
                                           order_bdb_triplet_db,
                                           text_file,
                                           errors,
                                           delimiter);
}

/*!
 * @brief select order from database based on order id
 * @param order_db order database to which to save
 * @param order_id order record key
 * @param order_dto order dto to save
 * @param errors if save fails
 */
void Order_DAO::lookup(Bdb_dbp &order_db,
                       const std::string &order_id,
                       Order_DTO &order_dto,
                       Bdb_errors &errors) {
  Order_DTO_key order_dto_key(order_id);
  Bdb_DAO::lookup<Order_DTO_key,
                  Order_DTO>(order_db, order_dto_key, order_dto, errors);
}

/*!
 * @brief save order dto to order database
 * @param order_db order database to which to save
 * @param order_dto prinicpals dto to save
 * @param errors if save fails
 */
void Order_DAO::save(Bdb_dbp &order_db, Order_DTO &order_dto, Bdb_errors &errors) {
  Order_DTO_key order_dto_key(order_dto);
  Bdb_DAO::save<Order_DTO_key,
                Order_DTO>(order_db, order_dto_key, order_dto, errors);
}

/*!
 * @brief select all order dto records from order database
 * @param order_db order database to select from
 * @param order_dto_list all order dtos in database
 * @param errors if select fails
 */
void Order_DAO::select_all(Bdb_dbp &order_db,
                           Order_DTO_list &order_dto_list,
                           Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(order_db, errors);
  if (!errors.has())
    bdb_cursor.dto_get_list<Order_DTO_key,
                            Order_DTO,
                            Order_DTO_list>(order_dto_list, errors);
}

/*!
 * @brief select order key list using order order_id to search order order_id->order key secondary database
 * @param order_order_id_sdb order order_id->order key secondary database
 * @param order_id secondary database search key
 * @param order_dto_key_list selected order key list
 * @param errors if order key not found
 */
void Order_DAO::select_all_for_account_id(Bdb_dbp &order_account_id_sdb,
                                          Bdb_dbp &order_db,
                                          const std::string &account_id,
                                          Order_DTO_list &order_dto_list,
                                          Bdb_errors &errors) {
  Order_DTO_account_id_key order_dto_account_id_key(account_id);
  Order_DTO_key_list order_dto_key_list;
  Bdb_cursor bdb_cursor(order_account_id_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Order_DTO_account_id_key,
                                      Order_DTO_key,
                                      Order_DTO_key_list>
        (order_dto_account_id_key,
         order_dto_key_list,
         errors, true);
  if (!errors.has())
    Bdb_DAO::select_by_key_list<Order_DTO_key,
                                Order_DTO_key_list,
                                Order_DTO,
                                Order_DTO_list>
        (order_db,
         order_dto_key_list,
         order_dto_list,
         errors);
}

/*!
 * @brief select order key list using order order_id to search order order_id->order key secondary database
 * @param order_order_id_sdb order order_id->order key secondary database
 * @param order_id secondary database search key
 * @param order_dto_key_list selected order key list
 * @param errors if order key not found
 */
void Order_DAO::select_all_for_name(Bdb_dbp &order_name_sdb,
                                    Bdb_dbp &order_db,
                                    const std::string &name,
                                    Order_DTO_list &order_dto_list,
                                    Bdb_errors &errors) {
  Order_DTO_name_key order_dto_name_key(name);
  Order_DTO_key_list order_dto_key_list;
  Bdb_cursor bdb_cursor(order_name_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Order_DTO_name_key,
                                      Order_DTO_key,
                                      Order_DTO_key_list>
        (order_dto_name_key,
         order_dto_key_list,
         errors);
  if (!errors.has())
    Bdb_DAO::select_by_key_list<Order_DTO_key,
                                Order_DTO_key_list,
                                Order_DTO,
                                Order_DTO_list>
        (order_db,
         order_dto_key_list,
         order_dto_list,
         errors);
}

void Order_DAO::update(Bdb_dbp &order_db,
                       Order_DTO &order_dto,
                       Bdb_errors &errors) {
  Order_DTO_key order_dto_key(order_dto);
  Bdb_DAO::save<Order_DTO_key, Order_DTO>
      (order_db, order_dto_key, order_dto, errors);
}
