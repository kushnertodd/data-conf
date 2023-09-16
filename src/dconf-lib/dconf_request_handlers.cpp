#include "bdb_common.hpp"
#include "bdb_json_utils.hpp"
#include "account_dao.hpp"
#include "account_dto.hpp"
#include "item_dao.hpp"
#include "item_dto.hpp"
#include "order_item_dao.hpp"
#include "order_dao.hpp"
#include "order_dto.hpp"
#include "dconf_inet_app_init.hpp"
#include "dconf_request.hpp"
#include "dconf_request_handlers.hpp"

void Dconf_request_handler::handle(Dconf_inet_app_init &dconf_inet_app_init,
                                   const Dconf_request &dconf_request,
                                   Dconf_request_response &dconf_request_response,
                                   Bdb_errors &errors) {
  if (!Dconf_account_request_handler::handle(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_item_request_handler::handle(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::handle(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_item_request_handler::handle(dconf_inet_app_init, dconf_request, dconf_request_response, errors))
    errors.add("Dconf_request_handler::handle", "", "invalid request: " + dconf_request.request);
}

// Dconf_account_request_handler methods

bool Dconf_account_request_handler::handle(Dconf_inet_app_init &dconf_inet_app_init,
                                           const Dconf_request &dconf_request,
                                           Dconf_request_response &dconf_request_response,
                                           Bdb_errors &errors) {
  if (!Dconf_account_request_handler::load(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_account_request_handler::lookup(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_account_request_handler::match_username(dconf_inet_app_init,
                                                        dconf_request,
                                                        dconf_request_response,
                                                        errors)
      && !Dconf_account_request_handler::select_all(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_account_request_handler::select_all_for_email(dconf_inet_app_init,
                                                              dconf_request,
                                                              dconf_request_response,
                                                              errors)
      && !Dconf_account_request_handler::select_all_for_username(dconf_inet_app_init,
                                                                 dconf_request,
                                                                 dconf_request_response,
                                                                 errors)
      && !Dconf_account_request_handler::update(dconf_inet_app_init, dconf_request, dconf_request_response, errors))
    return false;
  return true;
}

bool Dconf_account_request_handler::load(Dconf_inet_app_init &dconf_inet_app_init,
                                         const Dconf_request &dconf_request,
                                         Dconf_request_response &dconf_request_response,
                                         Bdb_errors &errors) {
  if (dconf_request.request != "account_load")
    return false;
  Dconf_request_handler::load<Account_DAO>(dconf_inet_app_init,
                                           dconf_request,
                                           "account",
                                           "account_username_triplets",
                                           dconf_request_response,
                                           errors);
  return true;
}

bool Dconf_account_request_handler::lookup(Dconf_inet_app_init &dconf_inet_app_init,
                                           const Dconf_request &dconf_request,
                                           Dconf_request_response &dconf_request_response,
                                           Bdb_errors &errors) {
  if (dconf_request.request != "account_lookup")
    return false;
  Dconf_request_handler::lookup<Account_DTO, Account_DAO>
      (dconf_inet_app_init,
       dconf_request,
       "account",
       dconf_request_response,
       errors);
  return true;
}

bool Dconf_account_request_handler::match_username(Dconf_inet_app_init &dconf_inet_app_init,
                                                   const Dconf_request &dconf_request,
                                                   Dconf_request_response &dconf_request_response,
                                                   Bdb_errors &errors) {
  if (dconf_request.request != "account_match_username")
    return false;
  Dconf_request_handler::match_text<Account_DTO, Account_DAO, Account_DTO_list>
      (dconf_inet_app_init,
       dconf_request,
       "account",
       "account_username_triplets",
       dconf_request_response,
       errors);
  return true;
}

bool Dconf_account_request_handler::select_all_for_email(Dconf_inet_app_init &dconf_inet_app_init,
                                                         const Dconf_request &dconf_request,
                                                         Dconf_request_response &dconf_request_response,
                                                         Bdb_errors &errors) {
  if (dconf_request.request != "account_select_all_for_email")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_request::select_all_for_email", "1", "missing email");
  std::string email;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor = std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database_config account_primary_database_config;
    dconf_inet_app_init.bdb_databases_config.select("account", account_primary_database_config, errors);
    Secondary_database_config account_email_secondary_database_config;
    if (!errors.has())
      account_primary_database_config.select("account_email",
                                             account_email_secondary_database_config,
                                             errors);
    if (!errors.has()) {
      Primary_database account_db(account_primary_database_config,
                                  dconf_bdb_key_extractor.get(),
                                  dconf_inet_app_init.db_home,
                                  errors);
      if (!errors.has()) {
        Secondary_database account_email_sdb(account_email_secondary_database_config,
                                             dconf_inet_app_init.db_home,
                                             errors);
        Account_DTO_list account_dto_list;
        if (!errors.has()) {
          email = dconf_request.arguments.at(0);
          Account_DAO::select_all_for_email(account_email_sdb.bdb_db,
                                            account_db.bdb_db,
                                            email,
                                            account_dto_list,
                                            errors);
        }
        if (!errors.has()) {
          json_object *account_dto_list_json = account_dto_list.to_json(errors);
          if (!errors.has()) {
            dconf_request_response.add_response(account_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Dconf_account_request_handler::select_all_for_username(Dconf_inet_app_init &dconf_inet_app_init,
                                                            const Dconf_request &dconf_request,
                                                            Dconf_request_response &dconf_request_response,
                                                            Bdb_errors &errors) {
  if (dconf_request.request != "account_select_all_for_username")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_request::select_all_for_username", "1", "missing username");
  std::string username;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor = std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database_config account_primary_database_config;
    dconf_inet_app_init.bdb_databases_config.select("account", account_primary_database_config, errors);
    Secondary_database_config account_username_secondary_database_config;
    if (!errors.has())
      account_primary_database_config.select("account_username",
                                             account_username_secondary_database_config,
                                             errors);
    if (!errors.has()) {
      Primary_database account_db(account_primary_database_config,
                                  dconf_bdb_key_extractor.get(),
                                  dconf_inet_app_init.db_home,
                                  errors);
      if (!errors.has()) {
        Secondary_database account_username_sdb(account_username_secondary_database_config,
                                                dconf_inet_app_init.db_home,
                                                errors);
        Account_DTO_list account_dto_list;
        if (!errors.has()) {
          username = dconf_request.arguments.at(0);
          Account_DAO::select_all_for_username(account_username_sdb.bdb_db,
                                               account_db.bdb_db,
                                               username,
                                               account_dto_list,
                                               errors);
        }
        if (!errors.has()) {
          json_object *account_dto_list_json = account_dto_list.to_json(errors);
          if (!errors.has()) {
            dconf_request_response.add_response(account_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Dconf_account_request_handler::select_all(Dconf_inet_app_init &dconf_inet_app_init,
                                               const Dconf_request &dconf_request,
                                               Dconf_request_response &dconf_request_response,
                                               Bdb_errors &errors) {
  if (dconf_request.request != "account_select_all")
    return false;
  Dconf_request_handler::select_all<Account_DTO_list, Account_DAO>(dconf_inet_app_init,
                                                                   "account",
                                                                   dconf_request_response,
                                                                   errors);
  return true;
}

bool Dconf_account_request_handler::update(Dconf_inet_app_init &dconf_inet_app_init,
                                           const Dconf_request &dconf_request,
                                           Dconf_request_response &dconf_request_response,
                                           Bdb_errors &errors) {
  if (dconf_request.request != "account_update")
    return false;
  if (dconf_request.arguments.size() < 4)
    errors.add("Dconf_account_request_handler::process_update_account_request", "1",
               "missing account id, username, email, or created date");
  Primary_database_config primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("account", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
        std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database
        account_db(primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string account_id = dconf_request.arguments.at(0);
      std::string username = dconf_request.arguments.at(1);
      std::string email = dconf_request.arguments.at(2);
      std::string created = dconf_request.arguments.at(3);
      Account_DTO account_dto(account_id, username, email, created);
      Account_DAO::update(account_db.bdb_db, account_dto, errors);
      if (!errors.has()) {
        json_object *account_json = account_dto.to_json(errors);
        if (!errors.has()) {
          dconf_request_response.add_response(account_json);
        }
      }
    }
  }
  return true;
}

// Dconf_item_request_handler methods

bool Dconf_item_request_handler::handle(Dconf_inet_app_init &dconf_inet_app_init,
                                        const Dconf_request &dconf_request,
                                        Dconf_request_response &dconf_request_response,
                                        Bdb_errors &errors) {
  if (!Dconf_item_request_handler::load(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_item_request_handler::lookup(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_item_request_handler::match_name(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_item_request_handler::select_all(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_item_request_handler::select_all_orders(dconf_inet_app_init,
                                                        dconf_request,
                                                        dconf_request_response,
                                                        errors)
      && !Dconf_item_request_handler::select_all_for_name(dconf_inet_app_init,
                                                          dconf_request,
                                                          dconf_request_response,
                                                          errors)
      && !Dconf_item_request_handler::select_all_for_cost(dconf_inet_app_init,
                                                          dconf_request,
                                                          dconf_request_response,
                                                          errors)
      && !Dconf_item_request_handler::update(dconf_inet_app_init, dconf_request, dconf_request_response, errors))
    return false;
  return true;
}

bool Dconf_item_request_handler::load(Dconf_inet_app_init &dconf_inet_app_init,
                                      const Dconf_request &dconf_request,
                                      Dconf_request_response &dconf_request_response,
                                      Bdb_errors &errors) {
  if (dconf_request.request != "item_load")
    return false;
  Dconf_request_handler::load<Item_DAO>(dconf_inet_app_init,
                                        dconf_request,
                                        "item",
                                        "item_name_triplets",
                                        dconf_request_response,
                                        errors);
  return true;
}

bool Dconf_item_request_handler::lookup(Dconf_inet_app_init &dconf_inet_app_init,
                                        const Dconf_request &dconf_request,
                                        Dconf_request_response &dconf_request_response,
                                        Bdb_errors &errors) {
  if (dconf_request.request != "item_lookup")
    return false;
  Dconf_request_handler::lookup<Item_DTO, Item_DAO>(dconf_inet_app_init,
                                                    dconf_request,
                                                    "item",
                                                    dconf_request_response,
                                                    errors);
  return true;
}

bool Dconf_item_request_handler::match_name(Dconf_inet_app_init &dconf_inet_app_init,
                                            const Dconf_request &dconf_request,
                                            Dconf_request_response &dconf_request_response,
                                            Bdb_errors &errors) {
  if (dconf_request.request != "item_match_name")
    return false;
  Dconf_request_handler::match_text<Item_DTO, Item_DAO, Item_DTO_list>
      (dconf_inet_app_init,
       dconf_request,
       "item",
       "item_name_triplets",
       dconf_request_response,
       errors);
  return true;
}

bool Dconf_item_request_handler::select_all(Dconf_inet_app_init &dconf_inet_app_init,
                                            const Dconf_request &dconf_request,
                                            Dconf_request_response &dconf_request_response,
                                            Bdb_errors &errors) {
  if (dconf_request.request != "item_select_all")
    return false;
  Dconf_request_handler::select_all<Item_DTO_list, Item_DAO>(dconf_inet_app_init,
                                                             "item",
                                                             dconf_request_response,
                                                             errors);
  return true;
}

bool Dconf_item_request_handler::select_all_orders(Dconf_inet_app_init &dconf_inet_app_init,
                                                   const Dconf_request &dconf_request,
                                                   Dconf_request_response &dconf_request_response,
                                                   Bdb_errors &errors) {
  if (dconf_request.request != "item_select_all_orders")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_item_request_handler::select_all_orders", "1", "missing item_id");
  Primary_database_config order_item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order_item", order_item_primary_database_config, errors);
  Secondary_database_config order_item_item_id_secondary_database_config;
  order_item_primary_database_config.select("order_item_item_id",
                                            order_item_item_id_secondary_database_config,
                                            errors);
  Primary_database_config order_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order", order_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
        std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database
        order_db(order_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
                 errors);
    Primary_database
        order_item_db(order_item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
                      errors);
    Secondary_database
        order_item_item_id_sdb(order_item_item_id_secondary_database_config, dconf_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string item_id = dconf_request.arguments.at(0);
      Order_DTO_list order_dto_list;
      Order_item_DAO::select_orders_for_item(order_item_item_id_sdb.bdb_db,
                                             order_item_db.bdb_db,
                                             order_db.bdb_db,
                                             item_id,
                                             order_dto_list,
                                             errors);
      if (!errors.has()) {
        json_object *item_dto_list_json = order_dto_list.to_json(errors);
        if (!errors.has()) {
          dconf_request_response.add_response(item_dto_list_json);
        }
      }
    }
  }
  return true;
}

bool Dconf_item_request_handler::select_all_for_name(Dconf_inet_app_init &dconf_inet_app_init,
                                                     const Dconf_request &dconf_request,
                                                     Dconf_request_response &dconf_request_response,
                                                     Bdb_errors &errors) {
  if (dconf_request.request != "item_select_all_for_name")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_item_request_handler::item_select_all_for_name",
               "1",
               "parameters: name");
  std::string name;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor = std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database_config item_primary_database_config;
    dconf_inet_app_init.bdb_databases_config.select("item", item_primary_database_config, errors);
    Secondary_database_config item_name_secondary_database_config;
    if (!errors.has())
      item_primary_database_config.select("item_name",
                                          item_name_secondary_database_config,
                                          errors);
    if (!errors.has()) {
      Primary_database item_db(item_primary_database_config,
                               dconf_bdb_key_extractor.get(),
                               dconf_inet_app_init.db_home,
                               errors);
      if (!errors.has()) {
        Secondary_database item_name_sdb(item_name_secondary_database_config,
                                         dconf_inet_app_init.db_home,
                                         errors);
        Item_DTO_list item_dto_list;
        if (!errors.has()) {
          name = dconf_request.arguments.at(0);
          Item_DAO::select_all_for_name(item_name_sdb.bdb_db,
                                        item_db.bdb_db,
                                        name,
                                        item_dto_list,
                                        errors);
        }
        if (!errors.has()) {
          json_object *item_dto_list_json = item_dto_list.to_json(errors);
          if (!errors.has()) {
            dconf_request_response.add_response(item_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Dconf_item_request_handler::select_all_for_cost(Dconf_inet_app_init &dconf_inet_app_init,
                                                     const Dconf_request &dconf_request,
                                                     Dconf_request_response &dconf_request_response,
                                                     Bdb_errors &errors) {
  if (dconf_request.request != "item_select_all_for_cost")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_item_request_handler::item_select_all_for_cost",
               "1",
               "parameters: cost");
  std::string cost;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor = std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database_config item_primary_database_config;
    dconf_inet_app_init.bdb_databases_config.select("item", item_primary_database_config, errors);
    Secondary_database_config item_cost_secondary_database_config;
    if (!errors.has())
      item_primary_database_config.select("item_cost",
                                          item_cost_secondary_database_config,
                                          errors);
    if (!errors.has()) {
      Primary_database item_db(item_primary_database_config,
                               dconf_bdb_key_extractor.get(),
                               dconf_inet_app_init.db_home,
                               errors);
      if (!errors.has()) {
        Secondary_database item_cost_sdb(item_cost_secondary_database_config,
                                         dconf_inet_app_init.db_home,
                                         errors);
        Item_DTO_list item_dto_list;
        if (!errors.has()) {
          cost = dconf_request.arguments.at(0);
          Item_DAO::select_all_for_cost(item_cost_sdb.bdb_db,
                                        item_db.bdb_db,
                                        cost,
                                        item_dto_list,
                                        errors);
        }
        if (!errors.has()) {
          json_object *item_dto_list_json = item_dto_list.to_json(errors);
          if (!errors.has()) {
            dconf_request_response.add_response(item_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Dconf_item_request_handler::update(Dconf_inet_app_init &dconf_inet_app_init,
                                        const Dconf_request &dconf_request,
                                        Dconf_request_response &dconf_request_response,
                                        Bdb_errors &errors) {
  if (dconf_request.request != "item_update")
    return false;
  if (dconf_request.arguments.size() < 3)
    errors.add("Dconf_item_request_handler::process_update_item_request", "1", "missing item_id");
  Primary_database_config primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("item", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
        std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database
        item_db(primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string item_id = dconf_request.arguments.at(0);
      std::string name = dconf_request.arguments.at(1);
      std::string cost = dconf_request.arguments.at(2);
      Item_DTO item_dto(item_id, name, cost);
      Item_DAO::update(item_db.bdb_db, item_dto, errors);
      if (!errors.has()) {
        json_object *item_json = item_dto.to_json(errors);
        if (!errors.has()) {
          dconf_request_response.add_response(item_json);
        }
      }
    }
  }
  return true;
}

// Dconf_order_request_handler methods

bool Dconf_order_request_handler::handle(Dconf_inet_app_init &dconf_inet_app_init,
                                         const Dconf_request &dconf_request,
                                         Dconf_request_response &dconf_request_response,
                                         Bdb_errors &errors) {
  if (!Dconf_order_request_handler::add_item(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::load(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::lookup(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::match_name(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::new_order(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::remove_item(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::remove_order(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::select_all(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      && !Dconf_order_request_handler::select_all_items(dconf_inet_app_init,
                                                        dconf_request,
                                                        dconf_request_response,
                                                        errors)
      && !Dconf_order_request_handler::select_all_for_account_id(dconf_inet_app_init,
                                                                 dconf_request,
                                                                 dconf_request_response,
                                                                 errors)
      && !Dconf_order_request_handler::select_all_for_name(dconf_inet_app_init,
                                                           dconf_request,
                                                           dconf_request_response,
                                                           errors)
      && !Dconf_order_request_handler::select_other_items(dconf_inet_app_init,
                                                          dconf_request,
                                                          dconf_request_response,
                                                          errors)
      && !Dconf_order_request_handler::update(dconf_inet_app_init, dconf_request, dconf_request_response, errors))
    return false;
  return true;
}

bool Dconf_order_request_handler::add_item(Dconf_inet_app_init &dconf_inet_app_init,
                                           const Dconf_request &dconf_request,
                                           Dconf_request_response &dconf_request_response,
                                           Bdb_errors &errors) {
  if (dconf_request.request != "order_add_item")
    return false;
  if (dconf_request.arguments.size() < 2)
    errors.add("Dconf_item_request_handler::add_item", "1", "missing order_id or item_id");
  Primary_database_config order_item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order_item", order_item_primary_database_config, errors);
  Secondary_database_config order_item_order_id_secondary_database_config;
  order_item_primary_database_config.select("order_item_order_id",
                                            order_item_order_id_secondary_database_config,
                                            errors);
  std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
      std::make_unique<Dconf_bdb_key_extractor>();
  Primary_database
      order_item_db(order_item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
                    errors);
  Secondary_database
      order_item_order_id_sdb(order_item_order_id_secondary_database_config, dconf_inet_app_init.db_home, errors);
  if (!errors.has()) {
    std::string order_id = dconf_request.arguments.at(0);
    std::string item_id = dconf_request.arguments.at(1);
    Order_item_DTO order_item_dto(order_id, item_id);
    Order_item_DTO order_item_dto_with_key;
    Order_item_DAO::save(order_item_db.bdb_db,
                         order_item_dto,
                         order_item_dto_with_key,
                         errors);
    if (!errors.has()) {
      json_object *item_dto_json = order_item_dto_with_key.to_json(errors);
      if (!errors.has()) {
        dconf_request_response.add_response(item_dto_json);
      }
    }
  }
  return true;
}

bool Dconf_order_request_handler::load(Dconf_inet_app_init &dconf_inet_app_init,
                                       const Dconf_request &dconf_request,
                                       Dconf_request_response &dconf_request_response,
                                       Bdb_errors &errors) {
  if (dconf_request.request != "order_load")
    return false;
  Dconf_request_handler::load<Order_DAO>(dconf_inet_app_init,
                                         dconf_request,
                                         "order",
                                         "order_name_triplets",
                                         dconf_request_response,
                                         errors);
  return true;
}

bool Dconf_order_request_handler::lookup(Dconf_inet_app_init &dconf_inet_app_init,
                                         const Dconf_request &dconf_request,
                                         Dconf_request_response &dconf_request_response,
                                         Bdb_errors &errors) {
  if (dconf_request.request != "order_lookup")
    return false;
  Dconf_request_handler::lookup<Order_DTO, Order_DAO>(dconf_inet_app_init,
                                                      dconf_request,
                                                      "order",
                                                      dconf_request_response,
                                                      errors);
  return true;
}

bool Dconf_order_request_handler::match_name(Dconf_inet_app_init &dconf_inet_app_init,
                                             const Dconf_request &dconf_request,
                                             Dconf_request_response &dconf_request_response,
                                             Bdb_errors &errors) {
  if (dconf_request.request != "order_match_name")
    return false;
  Dconf_request_handler::match_text<Order_DTO, Order_DAO, Order_DTO_list>
      (dconf_inet_app_init,
       dconf_request,
       "order",
       "order_name_triplets",
       dconf_request_response,
       errors);
  return true;
}

bool Dconf_order_request_handler::new_order(Dconf_inet_app_init &dconf_inet_app_init,
                                            const Dconf_request &dconf_request,
                                            Dconf_request_response &dconf_request_response,
                                            Bdb_errors &errors) {
  if (dconf_request.request != "order_new")
    return false;
  if (dconf_request.arguments.size() < 2)
    errors.add("Dconf_order_request_handler::new_order", "1", "missing name or account_id");
  Primary_database_config order_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order", order_primary_database_config, errors);
  std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
      std::make_unique<Dconf_bdb_key_extractor>();
  Primary_database
      order_db(order_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
               errors);
  if (!errors.has()) {
    std::string name = dconf_request.arguments.at(0);
    std::string account_id = dconf_request.arguments.at(1);
    Order_DTO order_dto(account_id, name);
    Order_DTO order_dto_with_key;
    Order_DAO::save_key(order_db.bdb_db,
                        order_dto,
                        order_dto_with_key,
                        errors);
    if (!errors.has()) {
      json_object *order_dto_json = order_dto_with_key.to_json(errors);
      if (!errors.has()) {
        dconf_request_response.add_response(order_dto_json);
      }
    }
  }
  return true;
}

bool Dconf_order_request_handler::remove_item(Dconf_inet_app_init &dconf_inet_app_init,
                                              const Dconf_request &dconf_request,
                                              Dconf_request_response &dconf_request_response,
                                              Bdb_errors &errors) {
  if (dconf_request.request != "order_remove_item")
    return false;
  if (dconf_request.arguments.size() < 2)
    errors.add("Dconf_item_request_handler::remove_item", "1", "missing order_id or item_id");
  Primary_database_config order_item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order_item", order_item_primary_database_config, errors);
  Secondary_database_config order_item_order_id_secondary_database_config;
  order_item_primary_database_config.select("order_item_order_id",
                                            order_item_order_id_secondary_database_config,
                                            errors);
  Secondary_database_config order_item_item_id_secondary_database_config;
  order_item_primary_database_config.select("order_item_item_id",
                                            order_item_item_id_secondary_database_config,
                                            errors);
  std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
      std::make_unique<Dconf_bdb_key_extractor>();
  Primary_database
      order_item_db(order_item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
                    errors);
  Secondary_database
      order_item_order_id_sdb(order_item_order_id_secondary_database_config, dconf_inet_app_init.db_home, errors);
  Secondary_database
      order_item_item_id_sdb(order_item_item_id_secondary_database_config, dconf_inet_app_init.db_home, errors);
  if (!errors.has()) {
    std::string order_id = dconf_request.arguments.at(0);
    std::string item_id = dconf_request.arguments.at(1);
    Order_item_DTO order_item_dto_with_key;
    Order_item_DAO::delete_order_item(order_item_db.bdb_db,
                                      order_item_order_id_sdb.bdb_db,
                                      order_item_item_id_sdb.bdb_db,
                                      order_id,
                                      item_id,
                                      order_item_dto_with_key,
                                      errors);
    if (!errors.has()) {
      json_object *item_dto_json = order_item_dto_with_key.to_json(errors);
      if (!errors.has()) {
        dconf_request_response.add_response(item_dto_json);
      }
    }
  }
  return true;
}

bool Dconf_order_request_handler::remove_order(Dconf_inet_app_init &dconf_inet_app_init,
                                               const Dconf_request &dconf_request,
                                               Dconf_request_response &dconf_request_response,
                                               Bdb_errors &errors) {
  if (dconf_request.request != "order_remove")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_item_request_handler::order_remove", "1", "missing order_id");
  Primary_database_config order_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order", order_primary_database_config, errors);
  Primary_database_config order_item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order_item", order_item_primary_database_config, errors);
  Secondary_database_config order_item_order_id_secondary_database_config;
  order_item_primary_database_config.select("order_item_order_id",
                                            order_item_order_id_secondary_database_config,
                                            errors);
  Secondary_database_config order_item_item_id_secondary_database_config;
  order_item_primary_database_config.select("order_item_item_id",
                                            order_item_item_id_secondary_database_config,
                                            errors);
  std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
      std::make_unique<Dconf_bdb_key_extractor>();
  Primary_database
      order_db(order_primary_database_config, dconf_bdb_key_extractor.get(),
               dconf_inet_app_init.db_home, errors);
  Primary_database
      order_item_db(order_item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
                    errors);
  Secondary_database
      order_item_order_id_sdb(order_item_order_id_secondary_database_config, dconf_inet_app_init.db_home, errors);
  Secondary_database
      order_item_item_id_sdb(order_item_item_id_secondary_database_config, dconf_inet_app_init.db_home, errors);
  if (!errors.has()) {
    std::string order_id = dconf_request.arguments.at(0);
    Order_DTO order_dto;
    Order_item_DAO::delete_order(order_db.bdb_db,
                                 order_item_db.bdb_db,
                                 order_item_order_id_sdb.bdb_db,
                                 order_id,
                                 order_dto,
                                 errors);
    if (!errors.has()) {
      json_object *item_dto_json = order_dto.to_json(errors);
      if (!errors.has()) {
        dconf_request_response.add_response(item_dto_json);
      }
    }
  }
  return true;
}

bool Dconf_order_request_handler::select_all(Dconf_inet_app_init &dconf_inet_app_init,
                                             const Dconf_request &dconf_request,
                                             Dconf_request_response &dconf_request_response,
                                             Bdb_errors &errors) {
  if (dconf_request.request != "order_select_all")
    return false;
  Dconf_request_handler::select_all<Order_DTO_list, Order_DAO>(dconf_inet_app_init,
                                                               "order",
                                                               dconf_request_response,
                                                               errors);
  return true;

}

bool Dconf_order_request_handler::select_all_items(Dconf_inet_app_init &dconf_inet_app_init,
                                                   const Dconf_request &dconf_request,
                                                   Dconf_request_response &dconf_request_response,
                                                   Bdb_errors &errors) {
  if (dconf_request.request != "order_select_all_items")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_order_request_handler::select_all_items", "1", "missing order_id");
  Primary_database_config order_item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order_item", order_item_primary_database_config, errors);
  Secondary_database_config order_item_order_id_secondary_database_config;
  order_item_primary_database_config.select("order_item_order_id",
                                            order_item_order_id_secondary_database_config,
                                            errors);
  Primary_database_config item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("item", item_primary_database_config, errors);
  std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
      std::make_unique<Dconf_bdb_key_extractor>();
  Primary_database
      item_db(item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home, errors);
  Primary_database
      order_item_db(order_item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
                    errors);
  Secondary_database
      order_item_order_id_sdb(order_item_order_id_secondary_database_config, dconf_inet_app_init.db_home, errors);
  if (!errors.has()) {
    std::string order_id = dconf_request.arguments.at(0);
    Item_DTO_list item_dto_list;
    Order_item_DAO::select_items_for_order(order_item_db.bdb_db,
                                           item_db.bdb_db,
                                           order_item_order_id_sdb.bdb_db,
                                           order_id,
                                           item_dto_list,
                                           errors);
    if (!errors.has()) {
      json_object *item_dto_list_json = item_dto_list.to_json(errors);
      if (!errors.has()) {
        dconf_request_response.add_response(item_dto_list_json);
      }
    }
  }
  return true;
}

bool Dconf_order_request_handler::select_all_for_account_id(Dconf_inet_app_init &dconf_inet_app_init,
                                                            const Dconf_request &dconf_request,
                                                            Dconf_request_response &dconf_request_response,
                                                            Bdb_errors &errors) {
  if (dconf_request.request != "order_select_all_for_account_id")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_order_request_handler::select_all_for_account_id",
               "1",
               "parameters: account_id");
  std::string account_id;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor = std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database_config order_primary_database_config;
    dconf_inet_app_init.bdb_databases_config.select("order", order_primary_database_config, errors);
    Secondary_database_config order_account_id_secondary_database_config;
    if (!errors.has())
      order_primary_database_config.select("order_account_id",
                                           order_account_id_secondary_database_config,
                                           errors);
    if (!errors.has()) {
      Primary_database order_db(order_primary_database_config,
                                dconf_bdb_key_extractor.get(),
                                dconf_inet_app_init.db_home,
                                errors);
      if (!errors.has()) {
        Secondary_database order_account_id_sdb(order_account_id_secondary_database_config,
                                                dconf_inet_app_init.db_home,
                                                errors);
        Order_DTO_list order_dto_list;
        if (!errors.has()) {
          account_id = dconf_request.arguments.at(0);
          Order_DAO::select_all_for_account_id(order_account_id_sdb.bdb_db,
                                               order_db.bdb_db,
                                               account_id,
                                               order_dto_list,
                                               errors);
        }
        if (!errors.has()) {
          json_object *order_dto_list_json = order_dto_list.to_json(errors);
          if (!errors.has()) {
            dconf_request_response.add_response(order_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Dconf_order_request_handler::select_all_for_name(Dconf_inet_app_init &dconf_inet_app_init,
                                                      const Dconf_request &dconf_request,
                                                      Dconf_request_response &dconf_request_response,
                                                      Bdb_errors &errors) {
  if (dconf_request.request != "order_select_all_for_name")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_order_request_handler::select_all_for_name",
               "1",
               "parameters: name");
  std::string name;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor = std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database_config order_primary_database_config;
    dconf_inet_app_init.bdb_databases_config.select("order", order_primary_database_config, errors);
    Secondary_database_config order_name_secondary_database_config;
    if (!errors.has())
      order_primary_database_config.select("order_name",
                                           order_name_secondary_database_config,
                                           errors);
    if (!errors.has()) {
      Primary_database order_db(order_primary_database_config,
                                dconf_bdb_key_extractor.get(),
                                dconf_inet_app_init.db_home,
                                errors);
      if (!errors.has()) {
        Secondary_database order_name_sdb(order_name_secondary_database_config,
                                          dconf_inet_app_init.db_home,
                                          errors);
        Order_DTO_list order_dto_list;
        if (!errors.has()) {
          name = dconf_request.arguments.at(0);
          Order_DAO::select_all_for_name(order_name_sdb.bdb_db,
                                         order_db.bdb_db,
                                         name,
                                         order_dto_list,
                                         errors);
        }
        if (!errors.has()) {
          json_object *order_dto_list_json = order_dto_list.to_json(errors);
          if (!errors.has()) {
            dconf_request_response.add_response(order_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Dconf_order_request_handler::select_other_items(Dconf_inet_app_init &dconf_inet_app_init,
                                                     const Dconf_request &dconf_request,
                                                     Dconf_request_response &dconf_request_response,
                                                     Bdb_errors &errors) {
  if (dconf_request.request != "order_select_other_items")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_item_request_handler::order_select_other_items", "1", "missing order_id");
  Primary_database_config order_item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order_item", order_item_primary_database_config, errors);
  Secondary_database_config order_item_order_id_secondary_database_config;
  order_item_primary_database_config.select("order_item_order_id",
                                            order_item_order_id_secondary_database_config,
                                            errors);
  Primary_database_config item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("item", item_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
        std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database
        item_db(item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
                errors);
    Primary_database
        order_item_db(order_item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home,
                      errors);
    Secondary_database
        order_item_order_id_sdb(order_item_order_id_secondary_database_config, dconf_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string order_id = dconf_request.arguments.at(0);
      Item_DTO_list item_dto_list;
      Order_item_DAO::select_other_items(order_item_order_id_sdb.bdb_db,
                                         order_item_db.bdb_db,
                                         item_db.bdb_db,
                                         order_id,
                                         item_dto_list,
                                         errors);
      if (!errors.has()) {
        json_object *item_dto_list_json = item_dto_list.to_json(errors);
        if (!errors.has()) {
          dconf_request_response.add_response(item_dto_list_json);
        }
      }
    }
  }
  return true;
}

bool Dconf_order_request_handler::update(Dconf_inet_app_init &dconf_inet_app_init,
                                         const Dconf_request &dconf_request,
                                         Dconf_request_response &dconf_request_response,
                                         Bdb_errors &errors) {
  if (dconf_request.request != "order_update")
    return false;
  if (dconf_request.arguments.size() < 3)
    errors.add("Dconf_order_request_handler::process_update_order_request", "1", "missing order_id");
  Primary_database_config primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
        std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database
        order_db(primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string order_id = dconf_request.arguments.at(0);
      std::string account_id = dconf_request.arguments.at(1);
      std::string name = dconf_request.arguments.at(2);
      Order_DTO order_dto(order_id, account_id, name);
      Order_DAO::update(order_db.bdb_db, order_dto, errors);
      if (!errors.has()) {
        json_object *order_json = order_dto.to_json(errors);
        if (!errors.has()) {
          dconf_request_response.add_response(order_json);
        }
      }
    }
  }
  return true;
}

// Dconf_order_item_request_handler methods

bool Dconf_order_item_request_handler::handle(Dconf_inet_app_init &dconf_inet_app_init,
                                              const Dconf_request &dconf_request,
                                              Dconf_request_response &dconf_request_response,
                                              Bdb_errors &errors) {
  if (!Dconf_order_item_request_handler::load(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
//      && !Dconf_order_item_request_handler::lookup(dconf_inet_app_init, dconf_request, dconf_request_response, errors)
      )
    return false;
  return true;
}

bool Dconf_order_item_request_handler::load(Dconf_inet_app_init &dconf_inet_app_init,
                                            const Dconf_request &dconf_request,
                                            Dconf_request_response &dconf_request_response,
                                            Bdb_errors &errors) {
  if (dconf_request.request != "order_item_load")
    return false;
  if (dconf_request.arguments.empty())
    errors.add("Dconf_order_item_request_handler::load", "1", "missing dconf load order_item request arguments");
  Primary_database_config order_item_primary_database_config;
  dconf_inet_app_init.bdb_databases_config.select("order_item", order_item_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> dconf_bdb_key_extractor =
        std::make_unique<Dconf_bdb_key_extractor>();
    Primary_database
        order_item_db
        (order_item_primary_database_config, dconf_bdb_key_extractor.get(), dconf_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string text_file = dconf_inet_app_init.tsv_home + "/" + dconf_request.arguments.at(0);
      int count = Order_item_DAO::load(order_item_db.bdb_db, text_file, errors, tab);
      if (!errors.has())
        dconf_request_response.add_response(Dconf_request_response::to_load_response(count, errors));
    }
  }
  return true;
}

// Dconf_workflow_request_handler methods
//
//bool Dconf_workflow_request_handler::handle(Dconf_inet_app_init &dconf_inet_app_init,
//                                            const Dconf_request &dconf_request,
//                                            Dconf_request_response &dconf_request_response,
//                                            Bdb_errors &errors) {
//  if (!Dconf_workflow_request_handler::remove_account(dconf_inet_app_init,
//                                                      dconf_request,
//                                                      dconf_request_response,
//                                                      errors)
//      && !Dconf_workflow_request_handler::remove_item(dconf_inet_app_init,
//                                                      dconf_request,
//                                                      dconf_request_response,
//                                                      errors)
//      && !Dconf_workflow_request_handler::remove_order(dconf_inet_app_init,
//                                                      dconf_request,
//                                                      dconf_request_response,
//                                                      errors)
//      && !Dconf_workflow_request_handler::add_item_to_order(dconf_inet_app_init,
//                                                            dconf_request,
//                                                            dconf_request_response,
//                                                            errors)
//      && !Dconf_workflow_request_handler::remove_order(dconf_inet_app_init,
//                                                       dconf_request,
//                                                       dconf_request_response,
//                                                       errors)
//      && !Dconf_workflow_request_handler::remove_item_from_order(dconf_inet_app_init,
//                                                                 dconf_request,
//                                                                 dconf_request_response,
//                                                                 errors))
//    return false;
//  return true;
//}
//
//bool Dconf_workflow_request_handler::remove_account(Dconf_inet_app_init &Dconf_inet_app_init,
//                                                    const Dconf_request &dconf_request,
//                                                    Dconf_request_response &dconf_request_response,
//                                                    Bdb_errors &errors) {
//
//  return false;
//}
//
//bool Dconf_workflow_request_handler::remove_item(Dconf_inet_app_init &dconf_inet_app_init,
//                                                 const Dconf_request &dconf_request,
//                                                 Dconf_request_response &dconf_request_response,
//                                                 Bdb_errors &errors) {
//  return true;
//}
//
//bool Dconf_workflow_request_handler::add_item_to_order(Dconf_inet_app_init &dconf_inet_app_init,
//                                                       const Dconf_request &dconf_request,
//                                                       Dconf_request_response &dconf_request_response,
//                                                       Bdb_errors &errors) {
//  return false;
//}
//
//bool Dconf_workflow_request_handler::remove_order(Dconf_inet_app_init &dconf_inet_app_init,
//                                                  const Dconf_request &dconf_request,
//                                                  Dconf_request_response &dconf_request_response,
//                                                  Bdb_errors &errors) {
//  return false;
//}
//
//bool Dconf_workflow_request_handler::remove_item_from_order(Dconf_inet_app_init &dconf_inet_app_init,
//                                                            const Dconf_request &dconf_request,
//                                                            Dconf_request_response &dconf_request_response,
//                                                            Bdb_errors &errors) {
//  return false;
//}
//
