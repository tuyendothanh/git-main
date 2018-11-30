/*  Cap.8   Data Types
    8.1     Numeric Types
    8.1.4   Serial Types

    psql -d mytestdb -U pgmihai
    \i 04_numeric_serial.sql
*/

/*  The data types smallserial,serial and bigserial are not true types.
    They are a notational convenience for creating unique identifier columns
    similar with AUTO_INCREMENT in other databases.
    
    Serial Type    Column Type   Size        Range
  --------------+--------------+----------+----------------------------------
    smallserial    smallint      2 bytes     1 to +32767
    serial         integer       4 bytes     1 to +2147483647
    bigserial      bigint        8 bytes     1 to +9223372036854775807                                      
   --------------------------------------------------------------------------
   
    By making id column a SERIAL, PostgreSQL will perform the following:
    - creates a sequence object and set the next value generated by the sequence
      as the default value for the column.
    - adds the NOT NULL constraint to the column because the sequence always
      generates an integer, which is a non-null value.
    - assigns the owner of the sequence to the id column; as a result, the
      sequence object is deleted when the id column or table id dropped.  
*/

DROP TABLE IF EXISTS fruits CASCADE;
CREATE TABLE fruits (
    id      SERIAL      PRIMARY KEY,
    name    VARCHAR     NOT NULL
    );
    
INSERT INTO fruits(name)        VALUES ('orange');
INSERT INTO fruits(id,name)     VALUES (DEFAULT, 'apple');
SELECT * FROM fruits;

/*
 id |  name
----+--------
  1 | orange
  2 | apple
(2 rows)
*/

\dt
/*
         List of relations
 Schema |  Name  | Type  |  Owner
--------+--------+-------+---------
 public | fruits | table | pgmihai
(1 row)
*/

\ds
/*
              List of relations
 Schema |     Name      |   Type   |  Owner
--------+---------------+----------+---------
 public | fruits_id_seq | sequence | pgmihai
(1 row)
*/

SELECT * FROM fruits_id_seq;
/*
 sequence_name | last_value | start_value | increment_by |      max_value      | min_value | cache_value | log_cnt | is_cycled | is_called
---------------+------------+-------------+--------------+---------------------+-----------+-------------+---------+-----------+-----------
 fruits_id_seq |          2 |           1 |            1 | 9223372036854775807 |         1 |           1 |      31 | f         | t
(1 row)
*/