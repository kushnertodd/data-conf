vim \
-c ":%s/deck/order/g" \
-c ":%s/Deck/Order/g" \
-c ":%s/card/item/g" \
-c ":%s/Card/Item/g" \
-c ":wq" \
$1
