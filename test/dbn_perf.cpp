//=======================================================================
// Copyright (c) 2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include <deque>

#include "catch.hpp"

#include "dll/dbn.hpp"

#include "mnist/mnist_reader.hpp"
#include "mnist/mnist_utils.hpp"

TEST_CASE( "dbn/mnist_100", "[dbn][bench][fast]" ) {
    typedef dll::dbn_desc<
        dll::dbn_layers<
            dll::rbm_desc<28 * 28, 100, dll::momentum, dll::batch_size<5>, dll::init_weights>::rbm_t,
            dll::rbm_desc<100, 200, dll::momentum, dll::batch_size<5>>::rbm_t,
            dll::rbm_desc<200, 10, dll::momentum, dll::batch_size<5>, dll::hidden<dll::unit_type::SOFTMAX>>::rbm_t>
        , dll::batch_size<5>
        >::dbn_t dbn_t;

    auto dataset = mnist::read_dataset<std::vector, std::vector, double>(25);

    REQUIRE(!dataset.training_images.empty());

    mnist::binarize_dataset(dataset);

    auto dbn = std::make_unique<dbn_t>();

    dbn->pretrain(dataset.training_images, 5);
    auto error = dbn->fine_tune(dataset.training_images, dataset.training_labels, 2);

    REQUIRE(error < 5e-2);

    auto test_error = dll::test_set(dbn, dataset.test_images, dataset.test_labels, dll::predictor());

    std::cout << "test_error:" << test_error << std::endl;

    REQUIRE(test_error < 0.2);
}

TEST_CASE( "dbn/mnist_101", "[dbn][bench][slow][parallel]" ) {
    typedef dll::dbn_desc<
        dll::dbn_layers<
            dll::rbm_desc<28 * 28, 300, dll::momentum, dll::parallel_mode, dll::batch_size<24>, dll::init_weights>::rbm_t,
            dll::rbm_desc<300, 1000, dll::momentum, dll::parallel_mode, dll::batch_size<24>>::rbm_t,
            dll::rbm_desc<1000, 10, dll::momentum, dll::parallel_mode, dll::batch_size<24>, dll::hidden<dll::unit_type::SOFTMAX>>::rbm_t>
        , dll::batch_size<5>
        >::dbn_t dbn_t;

    auto dataset = mnist::read_dataset<std::vector, std::vector, double>(2000);

    REQUIRE(!dataset.training_images.empty());

    mnist::binarize_dataset(dataset);

    auto dbn = std::make_unique<dbn_t>();

    dbn->pretrain(dataset.training_images, 20);
}

TEST_CASE( "dbn/mnist_102", "[dbn][bench][slow]" ) {
    typedef dll::dbn_desc<
        dll::dbn_layers<
            dll::rbm_desc<28 * 28, 300, dll::momentum, dll::batch_size<24>, dll::init_weights>::rbm_t,
            dll::rbm_desc<300, 1000, dll::momentum, dll::batch_size<24>>::rbm_t,
            dll::rbm_desc<1000, 10, dll::momentum, dll::batch_size<24>, dll::hidden<dll::unit_type::SOFTMAX>>::rbm_t>
        , dll::batch_size<5>
        >::dbn_t dbn_t;

    auto dataset = mnist::read_dataset<std::vector, std::vector, double>(2000);

    REQUIRE(!dataset.training_images.empty());

    mnist::binarize_dataset(dataset);

    auto dbn = std::make_unique<dbn_t>();

    dbn->pretrain(dataset.training_images, 20);
}