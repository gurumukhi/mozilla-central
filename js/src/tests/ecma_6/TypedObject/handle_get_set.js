// |reftest| skip-if(!this.hasOwnProperty("TypedObject"))

/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

var BUGNUMBER = 898342;
var summary = 'Handle Move';

var T = TypedObject;

var Point = new T.ArrayType(T.float32, 3);
var Line = new T.StructType({from: Point, to: Point});
var Lines = new T.ArrayType(Line, 3);

function runTests() {
  function testHandleGetSetWithScalarType() {
    var lines = new Lines([
      {from: [1, 2, 3], to: [4, 5, 6]},
      {from: [7, 8, 9], to: [10, 11, 12]},
      {from: [13, 14, 15], to: [16, 17, 18]}
    ]);

    var handle = T.float32.handle(lines, 0, "to", 1);
    assertEq(T.Handle.get(handle), 5);
    T.Handle.set(handle, 22);
    assertEq(T.Handle.get(handle), 22);
    assertEq(lines[0].to[1], 22);
  }
  testHandleGetSetWithScalarType();

  function testHandleGetSetWithComplexType() {
    var lines = new Lines([
      {from: [1, 2, 3], to: [4, 5, 6]},
      {from: [7, 8, 9], to: [10, 11, 12]},
      {from: [13, 14, 15], to: [16, 17, 18]}
    ]);

    var handle = Point.handle(lines, 0, "to");

    T.Handle.set(handle, [22, 23, 24]);

    assertEq(handle[0], 22);
    assertEq(handle[1], 23);
    assertEq(handle[2], 24);

    assertEq(T.Handle.get(handle)[0], 22);
    assertEq(T.Handle.get(handle)[1], 23);
    assertEq(T.Handle.get(handle)[2], 24);
  }
  testHandleGetSetWithComplexType();

  reportCompare(true, true);
  print("Tests complete");
}

runTests();


