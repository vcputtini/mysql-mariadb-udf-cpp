-- =========================================================================
--
--
--
-- Syntax:
-- CREATE [OR REPLACE] [AGGREGATE] FUNCTION [IF NOT EXISTS] function_name
--    RETURNS {STRING|INTEGER|REAL|DECIMAL}
--    SONAME shared_library_name
-- =========================================================================

-- USE default;

-- Install UDFs
CREATE OR REPLACE FUNCTION geo_distance RETURNS REAL SONAME 'liblocation.so';
CREATE OR REPLACE FUNCTION geo_azimuth RETURNS REAL SONAME 'liblocation.so';
CREATE OR REPLACE FUNCTION unit_convert RETURNS REAL SONAME 'liblocation.so';

-- Install UDAs - Aggragations
CREATE OR REPLACE AGGREGATE FUNCTION geo_sumdist RETURNS REAL SONAME 'liblocation.so';
