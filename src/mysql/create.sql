create table if not exists business_order (
  id bigint unsigned not null auto_increment,
  appid varchar(32) not null,
  mchid varchar(32) not null,
  out_trade_no varchar(32) not null,
  transaction_id varchar(32) not null,
  trade_type varchar(16) not null,
  trade_state varchar(32) not null,
  trade_state_desc varchar(256) not null,
  bank_type varchar(32) not null,
  attach varchar(128),
  success_time varchar(64) not null,
  open_id varchar(128) not null,
  total int unsigned not null,
  payer_total int unsigned not null,
  currency varchar(16) not null,
  payer_currency varchar(16) not null,
  primary key(id),
  unique key uk_no(mchid,out_trade_no)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

insert into business_order (
  appid,mchid,out_trade_no,transaction_id,
  trade_type,trade_state,trade_state_desc,
  bank_type,attach,success_time,open_id,total,
  payer_total,currency,payer_currency)
  values
  ("wxd678efh567hg6787","1230000109","1217752501201407033233368018","1217752501201407033233368018",
  "MICROPAY","SUCCESS","支付成功","CMC","","2018-06-08T10:34:56+08:00",
  "oUpF8uMuAJO_M2pxb1Q9zNjWeS6o",100,100,"CNY","CNY");